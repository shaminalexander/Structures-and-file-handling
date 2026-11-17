/******************
* Шамин Александр *
* ПИ-261          *
* Варивнт 35      *
*******************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

struct Equipment {
    string name;
    string type;
    double weight;
    int quantity;
    double price;
};

bool readFile(string fileName, vector<Equipment>& equipmentList, int& errorLine) {
    ifstream inputFile(fileName);
    string line;
    int lineNumber = 0;

    if (!inputFile) {
        cout << "Cannot open input file.\n";
        return false;
    }

    while (getline(inputFile, line)) {
        lineNumber++;

        if (equipmentList.size() >= 100) {
            errorLine = lineNumber;
            return false;
        }

        string fields[5];
        stringstream lineStream(line);
        string field;
        int fieldCount = 0;

        while (getline(lineStream, field, ';')) {
            if (fieldCount >= 5) {
                errorLine = lineNumber;
                return false;
            }

            fields[fieldCount] = field;
            fieldCount++;
        }

        if (fieldCount != 5 || fields[0].empty() || fields[1].empty()) {
            errorLine = lineNumber;
            return false;
        }

        Equipment current;
        string extraText;

        stringstream weightStream(fields[2]);
        stringstream quantityStream(fields[3]);
        stringstream priceStream(fields[4]);

        if (!(weightStream >> current.weight) || (weightStream >> extraText)) {
            errorLine = lineNumber;
            return false;
        }

        if (!(quantityStream >> current.quantity) || (quantityStream >> extraText)) {
            errorLine = lineNumber;
            return false;
        }

        if (!(priceStream >> current.price) || (priceStream >> extraText)) {
            errorLine = lineNumber;
            return false;
        }

        if (current.weight < 0 || current.quantity < 0 || current.price < 0) {
            errorLine = lineNumber;
            return false;
        }

        current.name = fields[0];
        current.type = fields[1];

        equipmentList.push_back(current);
    }

    if (inputFile.bad()) {
        cout << "Error while reading input file.\n";
        return false;
    }

    return true;
}

void makeReport(const vector<Equipment>& equipmentList, string requestedType, ostringstream& report) {
    double totalWeight = 0;
    double totalRevenue = 0;
    int foundCount = 0;
    int lightestIndex = -1;
    int equipmentCount = equipmentList.size();

    report << fixed << setprecision(2);
    report << "Equipment of type: " << requestedType << "\n\n";

    for (int equipmentIndex = 0; equipmentIndex < equipmentCount; equipmentIndex++) {
        if (equipmentList[equipmentIndex].type == requestedType) {
            report << "Name: " << equipmentList[equipmentIndex].name << "\n";
            report << "Type: " << equipmentList[equipmentIndex].type << "\n";
            report << "Weight: " << equipmentList[equipmentIndex].weight << " kg\n";
            report << "Quantity: " << equipmentList[equipmentIndex].quantity << "\n";
            report << "Daily price: " << equipmentList[equipmentIndex].price << " rubles\n\n";

            totalWeight += equipmentList[equipmentIndex].weight * equipmentList[equipmentIndex].quantity;
            totalRevenue += equipmentList[equipmentIndex].price * equipmentList[equipmentIndex].quantity;
            foundCount++;
        }

        if (lightestIndex == -1 ||
            equipmentList[equipmentIndex].weight < equipmentList[lightestIndex].weight) {
            lightestIndex = equipmentIndex;
        }
    }

    if (foundCount == 0) {
        report << "No equipment of this type was found.\n";
    }

    report << "Total weight: " << totalWeight << " kg\n";
    report << "Daily revenue: " << totalRevenue << " rubles\n\n";
    report << "The lightest equipment:\n";

    if (lightestIndex == -1) {
        report << "NONE\n";
    }
    else {
        report << "Name: " << equipmentList[lightestIndex].name << "\n";
        report << "Weight: " << equipmentList[lightestIndex].weight << " kg\n";
    }
}

int main() {
    string inputPath;
    string outputPath;
    string requestedType;

    vector<Equipment> equipmentList;
    int errorLine = 0;

    inputPath = "input.txt";
    outputPath = "output.txt";
    requestedType = "Sleeping equipment";

    if (!readFile(inputPath, equipmentList, errorLine)) {
        if (errorLine != 0) {
            cout << "ERROR line " << errorLine << "\n";
        }

        return 1;
    }

    ostringstream report;
    makeReport(equipmentList, requestedType, report);

    cout << "\n" << report.str();

    ofstream outputFile(outputPath);

    if (!outputFile) {
        cout << "Cannot open output file.\n";
        return 1;
    }

    outputFile << report.str();

    if (!outputFile) {
        cout << "Error writing output file.\n";
        return 1;
    }

    cout << "\nReport was saved.\n";

    return 0;
}
