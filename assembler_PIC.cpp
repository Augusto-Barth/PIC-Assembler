#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;


struct instrucao {
    string mnemonic, oper1, oper2;
};

vector<instrucao> programa;

int numArgs(string &op){
    if(op == "NOP" || op == "CLRW")
        return 0;
    else if(op == "MOVWF" || op == "CLRF" || op == "RETLW" || op == "CALL" || op == "GOTO" || op == "MOVLW" \
    || op == "IORLW" || op == "ANDLW" || op == "XORLW")
        return 1;
    return 2;
}

// quantos bits o argumento arg tem na operacao op
int tamanhoArgs(string &op, int arg){
    if(op == "MOVWF" || op == "CLRF" )
        return 5;

    else if(op == "RETLW" || op == "CALL" || op == "GOTO" || op == "MOVLW" \
    || op == "IORLW" || op == "ANDLW" || op == "XORLW")
        return 8;
    
    else if(op == "SUBWF" || op == "DECF" || op == "IORWF" || op == "ANDWF" \
    || op == "XORWF" || op == "ADDWF" || op == "MOVF" || op == "COMF" || op == "INCF" \
    || op == "DECFSZ" || op == "RRF" || op == "RLF" || op == "SWAPF" || op == "INCFSZ"){
        if(arg == 1){
            return 5;
        }
        else if(arg == 2){
            return 1;
        }
    }
    else{
        if(arg == 1){
            return 5;
        }
        else if(arg == 2){
            return 3;
        }
    }
    return 0;
}

void escreveInstrucao(ofstream &outputFile, instrucao &instr){
    string mnemonic = instr.mnemonic;
    outputFile << "\"";

    if(mnemonic == "NOP"){
        outputFile << "000000000000";
    }
    else if(mnemonic == "MOVWF"){
        outputFile << "0000001" << instr.oper1;
    }
    else if(mnemonic == "CLRW"){
        outputFile << "000001000000";
    }
    else if(mnemonic == "CLRF"){
        outputFile << "0000011" << instr.oper1;
    }
    else if(mnemonic == "SUBWF"){
        outputFile << "000010" << instr.oper2 << instr.oper1;
    }
    else if(mnemonic == "DECF"){
        outputFile << "000011" << instr.oper2 << instr.oper1;
    }
    else if(mnemonic == "IORWF"){
        outputFile << "000100" << instr.oper2 << instr.oper1;
    }
    else if(mnemonic == "ANDWF"){
        outputFile << "000101" << instr.oper2 << instr.oper1;
    }
    else if(mnemonic == "XORWF"){
        outputFile << "000110" << instr.oper2 << instr.oper1;
    }
    else if(mnemonic == "ADDWF"){
        outputFile << "000111" << instr.oper2 << instr.oper1;
    }
    else if(mnemonic == "MOVF"){
        outputFile << "001000" << instr.oper2 << instr.oper1;
    }
    else if(mnemonic == "COMF"){
        outputFile << "001001" << instr.oper2 << instr.oper1;
    }
    else if(mnemonic == "INCF"){
        outputFile << "001010" << instr.oper2 << instr.oper1;
    }
    else if(mnemonic == "DECFSZ"){
        outputFile << "001011" << instr.oper2 << instr.oper1;
    }
    else if(mnemonic == "RRF"){
        outputFile << "001100" << instr.oper2 << instr.oper1;
    }
    else if(mnemonic == "RLF"){
        outputFile << "001101" << instr.oper2 << instr.oper1;
    }
    else if(mnemonic == "SWAPF"){
        outputFile << "001110" << instr.oper2 << instr.oper1;
    }
    else if(mnemonic == "INCFSZ"){
        outputFile << "001111" << instr.oper2 << instr.oper1;
    }
    else if(mnemonic == "BCF"){
        outputFile << "0100" << instr.oper2 << instr.oper1;
    }
    else if(mnemonic == "BSF"){
        outputFile << "0101" << instr.oper2 << instr.oper1;
    }
    else if(mnemonic == "BTFSC"){
        outputFile << "0110" << instr.oper2 << instr.oper1;
    }
    else if(mnemonic == "BTFSS"){
        outputFile << "0111" << instr.oper2 << instr.oper1;
    }
    else if(mnemonic == "RETLW"){
        outputFile << "1000" << instr.oper1;
    }
    else if(mnemonic == "CALL"){
        outputFile << "1001" << instr.oper1;
    }
    else if(mnemonic == "GOTO"){
        outputFile << "1010" << instr.oper1;
    }
    else if(mnemonic == "MOVLW"){
        outputFile << "1100" << instr.oper1;
    }
    else if(mnemonic == "IORLW"){
        outputFile << "1101" << instr.oper1;
    }
    else if(mnemonic == "ANDLW"){
        outputFile << "1110" << instr.oper1;
    }
    else if(mnemonic == "XORLW"){
        outputFile << "1111" << instr.oper1;
    }
    

    outputFile << "\"";
}

string decToBin(int dec, int bits){
    string binary ("");
    int mask = 1;
    for(int i = 0; i < bits; i++){
        if((mask&dec) >= 1)
            binary = "1"+binary;
        else
            binary = "0"+binary;
        mask<<=1;
    }
    return binary;
}

void toUpper(string &str){
    for(int i = 0; i < str.size(); i++){
        str[i] = toupper(str[i]);
    }
}

// FAZER TO UPPER NAS INSTRUCOES

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Uso: %s [arquivo.pic]\n", argv[0]);
        return 1;
    }

    string inputName = string(argv[1]);
    ifstream inputFile(inputName, ifstream::in);

    string instr;

    while(inputFile.peek() != EOF){
        instrucao instr;
        inputFile >> instr.mnemonic;
        toUpper(instr.mnemonic);
        int numArgumentos = numArgs(instr.mnemonic);
        if(numArgumentos == 1){
            inputFile >> instr.oper1;
            if(!instr.oper1.compare(0,1,"#")){
                instr.oper1.erase(0, 1);
                instr.oper1 = decToBin(stoi(instr.oper1), tamanhoArgs(instr.mnemonic, 1));
            }
            else if(!instr.oper1.compare(0,2,"0x")){
                instr.oper1.erase(0, 2);
                instr.oper1 = decToBin(stoi(instr.oper1, nullptr, 16), tamanhoArgs(instr.mnemonic, 1));
            }
        }
        else if(numArgumentos == 2){
            inputFile >> instr.oper1;
            inputFile >> instr.oper2;
            if(!instr.oper1.compare(0,1,"#")){
                instr.oper1.erase(0, 1);
                instr.oper1 = decToBin(stoi(instr.oper1), tamanhoArgs(instr.mnemonic, 1));
            }
            else if(!instr.oper1.compare(0,2,"0x")){
                instr.oper1.erase(0, 2);
                instr.oper1 = decToBin(stoi(instr.oper1, nullptr, 16), tamanhoArgs(instr.mnemonic, 1));
            }
            if(!instr.oper2.compare(0,1,"#")){
                instr.oper2.erase(0, 1);
                instr.oper2 = decToBin(stoi(instr.oper2), tamanhoArgs(instr.mnemonic, 2));
            }
            else if(!instr.oper2.compare(0,2,"0x")){
                instr.oper2.erase(0, 2);
                instr.oper2 = decToBin(stoi(instr.oper2, nullptr, 16), tamanhoArgs(instr.mnemonic, 2));
            }
        }
        // toUpper(instr.oper1);
        // toUpper(instr.oper2);
        programa.push_back(instr);
    }
    inputFile.close();
    
    string outputName = inputName;
    outputName.replace(outputName.end()-3, outputName.end(), "out");
    ofstream outputFile(outputName, ofstream::out);

    int tamanhoPrograma = (int)programa.size();
    for(int i = 0; i < tamanhoPrograma; i++){
        escreveInstrucao(outputFile, programa[i]);
        if(i != tamanhoPrograma - 1){
            outputFile << ",\n";
        }
    }
    outputFile.close();

    return 0;
}
