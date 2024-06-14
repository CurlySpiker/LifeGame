// TODO: Add headers

#include "board.h"
#include <algorithm>
#include <filesystem>

using std::string;
using std::vector;
using std::cout;
using std::endl;


namespace life {

    size_t BOARD_MAX_SIZE = 256*256;

    Board::Board(string filepath, bool noLimit)
    {
        mNoLimit = noLimit;
        mInputName = filepath;

        // Extract input name from file path
        std::filesystem::path p(filepath);
        mInputName = p.stem();

        // Set output directory (hardcoded for now, subject to improvement)
        mOutputDir = std::filesystem::absolute("./lifegame_output");
        std::filesystem::create_directory(mOutputDir);

        // Check that input file exists
        std::ifstream ifs(filepath);
        if (!ifs.good()) {
            throw std::runtime_error("File does not exist: " + filepath);
        }

        // Parse input file
        string line;
        while (std::getline(ifs, line))
        {
            // Check that all lines have the same length
            if (mW <= 0) {
                mW = line.size();
            }
            else {
                if (line.size() != mW) {
                    string error = "Input file has lines with different lengths: line ";
                    error += std::to_string(mCells.size() + 1);
                    throw std::logic_error(error);
                }
            }

            // Populate board array base on file characters
            std::vector<bool> cellsRow;
            for (const auto& cellStatus: line) {
                switch(cellStatus) {
                    case '_':
                        cellsRow.push_back(false);
                        break;
                    case '*':
                        cellsRow.push_back(true);
                        break;
                    default:
                        string error = "Found unknown cell status: ";
                        error += string(1, cellStatus) + " (line " +
                            std::to_string(mCells.size() + 1)  + ")";

                        throw std::logic_error(error);
                }
            }

            mCells.push_back(cellsRow);
        }

        mH = mCells.size();

        // Shrink the board to correct for bad input files with too many empty cells
        shrink();
    }


    bool Board::expand() {

        // Abort expansion if size limit was reached and safeguard was not overridden.
        if (mW * mH > BOARD_MAX_SIZE && !mNoLimit) {
            return false;
        }

        // If board is empty, make it a one-cell array (with dead cell)
        if (mW == 0 || mH == 0){
            mCells.push_back({false});
            mH = 1;
            mW = 1;
            return true;
        }

        // And empty columns on left and right sides
        for (auto& row : mCells) {
            row.insert(row.begin(), false);
            row.insert(row.end(), false);
        }
        mW += 2;

        // And empty lines on top and bottom sides
        mCells.insert(mCells.begin(), vector<bool>(mW, false));
        mCells.insert(mCells.end(), vector<bool>(mW, false));
        mH += 2;

        return true;
    }


    void Board::shrink() {

        // Compute crop limits
        size_t firstY = mH; // Lowest row containing at least 1 live cell
        size_t lastY = 0; // Highest row containing at least 1 live cell
        size_t firstX = mW; // Lowest col containing at least 1 live cell
        size_t lastX = 0; // Hgihest col containing at least 1 live cell

        for (size_t y = 0; y < mH; y++) {

            // Get index of first and last live cells in this row
            auto firstAliveCell = find(mCells[y].begin(),mCells[y].end(), true);
            auto lastAliveCell = find(mCells[y].rbegin(),mCells[y].rend(), true);

            auto firstAliveX = std::distance(mCells[y].begin(), firstAliveCell);
            auto lastAliveX = mW - std::distance(mCells[y].rbegin(), lastAliveCell);

            // Adjust overall crop limits
            if (firstAliveX != mW) {
                lastY = y+1;
                if (firstY == mH) {
                    firstY = y;
                }

                if (lastAliveX > lastX) {
                    lastX = lastAliveX;
                }
                if (firstAliveX < firstX) {
                    firstX = firstAliveX;
                }
            }
        }

        // Handle empty board exception special case
        if(firstY == mH) {
            mCells.clear();
            mH = 0;
            mW = 0;
        }
        else {
            // Vertical crop
            vector<vector<bool>>(mCells.begin() + firstY, mCells.begin() + lastY).swap(mCells);

            // Horizontal crop
            for (auto& row: mCells) {
                vector<bool>(row.begin() + firstX, row.begin() + lastX).swap(row);
            }

            // Adjust size variables
            mH = mCells.size();
            mW = mCells[0].size();
        }
    }


    void Board::iterate(int nIter, bool saveAll, int currentIter) {

        cout << "Iteration: " << currentIter << "\tBoard size: "
            << mW << "*" << mH << endl;

        // If board is empty, no more life can come out of it. Stop here.
        if (mW == 0 || mH == 0) {
            cout << "Board is empty, stopping iteration!" << endl;
            return;
        }

        /* Save board to output file if required. If saving operation fails, abort the
        /* iteration process as it will not be observable anyway.*/
        if((saveAll || currentIter == nIter)) {
            auto name = mInputName + "_" + std::to_string(currentIter) + ".txt";
            if (!save(name)) {
                cout << "Could not save file !" << endl;
                cout << "Aborting..." << endl;
                return;
            }
        }

        // Stop the recursive loop when reaching the desired number of iterations
        if (currentIter>=nIter) {
            cout << "Iteration over!" << endl;
            return;
        }

        // Start by expanding the board to make room for potential new live cells
        if (!expand()) {
            cout << "Board is too big, stopping iteration!" << endl;
            return;
        }

        vector<vector<bool>> newCells(mCells); // Duplicate board, will receive next gen

        // Process with cells status update
        for (size_t y = 0; y < mH; y++) {
            for (size_t x = 0; x < mW; x++) {

                auto alive_neighbours = countAliveNeighbours(x, y);
                const auto& alive = mCells[y][x];

                // Update alive cells that should die
                if (alive && alive_neighbours != 2 && alive_neighbours != 3) {
                    newCells[y][x] = false;
                }
                // Update dead cells that should live
                else if (!alive && alive_neighbours==3) {
                    newCells[y][x] = true;
                }
                // Leave the rest unchanged
            }
        }

        // Update board with new state
        newCells.swap(mCells);

        // Ensure minimal size
        shrink();

        // Repeat iteration recursively until desired number of steps is reached
        iterate(nIter, saveAll, currentIter+1);
    }


    string Board::toString() {

        string output = "";
        for (const auto& row : mCells) {
            for (const auto& cellStatus : row) {
                output += (cellStatus ? '*' : '_');
            }
            output += "\n";
        }
        return output;
    }


    uint8_t Board::countAliveNeighbours(size_t x, size_t y) {

        uint8_t count = 0;
        for (int8_t dx=-1; dx<=1; dx++) {
            for (int8_t dy=-1; dy<=1; dy++) {
                if (dx == 0 && dy == 0) {
                    continue;
                }

                if (x+dx < 0 || x+dx >= mW || y+dy < 0 || y+dy >= mH) {
                    continue;
                }

                count += mCells[y+dy][x+dx];
            }
        }
        return count;
    }


    bool Board::save(string name) {
        try{
            auto filepath = std::filesystem::path(mOutputDir) / name;
            std::ofstream outputFile(filepath);
            outputFile << toString();
            outputFile.close();
        } catch(...) {
            return false;
        }
        return true;
    }
}
