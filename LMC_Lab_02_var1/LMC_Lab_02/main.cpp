#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <vector>
#include <map>
#include <cmath>

// Operatii logice
int impl(int a, int b) {
    if ((a == 0) || (b == 1)) return 1;
    return 0;
}

int disj(int a, int b) {
    if ((a == 0) && (b == 0)) return 0;
    return 1;
}

int conj(int a, int b) {
    if ((a == 1) && (b == 1)) return 1;
    return 0;
}

int echiv(int a, int b) {
    if (a == b) return 1;
    return 0;
}

int neg(int a) {
    if (a == 0) return 1;
    return 0;
}

// Verifica operatorii logici
bool verifElLeg(char a) {
    char elLeg[] = {'~', 'n', 'V', '&', '-'};
    for (int i = 0; i < 5; i++) if (a == elLeg[i]) return 1;
    return 0;
}

// Verifica variabile
char verifVar(char a) {
    char var[] = {'p', 'q', 'r', 's', 't', 'l', 'm', 'o', 'j'};
    for (int i = 0; i < 10; i++) if (a == var[i]) return var[i];
    return '0';
}

// Prioritatea operatiilor
int prioritate(char op) {
    if (op == 'n') return 5; // Negatie
    if (op == '&') return 4; // Conjunctie
    if (op == 'V') return 3; // Disjunctie
    if (op == '-') return 2; // Implicatie
    if (op == '~') return 1; // Echivalenta
    return 0;
}

// Asociativitatea operatorilor logici (1 pentru asociativitate-stanga, -1 pentru asociativitate-dreapta)
int asociativitate(char op) {
    if (op == 'n') return -1; // asociativitate-dreapta
    return 1; // asociativitate-stanga pentru alti operatori logici
}

// Converteste forma infixata in forma postfixata (poloneza inversa) (Shunting Yard Algorithm)
std::string infixToPostfix(std::string infix) {
    std::stack<char> operatori;
    std::string output = "";
    for (char token : infix) {
        if (verifVar(token) != '0') {
            // Daca avem o variabila, o adaugam la stiva
            output += token;
        } else if (verifElLeg(token)) {
            // Daca e un operator
            while (!operatori.empty() && verifElLeg(operatori.top()) && //atata timp cat stiva de operatori nu e goala si operatorul logic din varf nu e paranteza
                   ((asociativitate(token) == 1 && prioritate(token) <= prioritate(operatori.top())) || //asociativ la stanga si operatorul logic actual nu este mai prioritar decat cel din varful stivei
                    (asociativitate(token) == -1 && prioritate(token) < prioritate(operatori.top())))) { //asociativ la dreapta si operatorul logic curent nu este mai prioritar strict decat cel din varful stivei
                output += operatori.top(); //ia operatorul logic care se afla in varful stivei de operatori si il adauga la output (forma postfixata)
                operatori.pop(); //sterge operatorul pe care tocmai l-am trimis la output din varful stivei de operatori
            }
            operatori.push(token); //adauga la stiva de operatori logici operatorul curent cu prioritate mai mica
        } else if (token == '(') { //daca el curent este paranteza deschisa
            operatori.push(token); //adauga la stiva de operatori logici '('
        } else if (token == ')') { //daca el curent e paranteza inchisa
            while (!operatori.empty() && operatori.top() != '(') { //atata timp cat stiva de operatori logici nu este goala si operatorul din varf e diferit de paranteza deschisa
                output += operatori.top(); //adauga la forma postfixata operatorul logic
                operatori.pop(); //sterge operatorul logic (care a fost adaugat la forma postfixata) din stiva
            } // daca se iese din while, ori stiva de operatori e goala, ori s-a gasit o paranteza deschisa
            operatori.pop(); // se sterge paranteza deschisa
        }
    }

    // introduce in forma postfixata operatorii logici ramasi in stiva
    while (!operatori.empty()) {
        output += operatori.top();
        operatori.pop();
    }

    return output;
}

// Evaluarea formei postfixate
int evaluarePostfix(const std::string &postfix, std::unordered_map<char, int> &values) {
    std::stack<int> evalStiva; // crearea stivei de evaluare a formulei

    for (char token : postfix) { // se iau elementele din forma postfixata de la stanga la dreapta
        if (verifVar(token) != '0') { // daca elementul urmator din forma postfixata este o variabila
            evalStiva.push(values[token]); // se introduce valoarea variabilei in stiva de evaluare, de la stanga la dreapta
        } else if (token == 'n') { // daca elementul urmator din forma postfixata este operatorul logic negatie
            int a = evalStiva.top(); // se initializeaza 'a' (variabila tip intreg) care va primi valoarea variabilei din varful stivei de evaluare
            evalStiva.pop(); // se elimina din stiva de evaluarea valoarea variabilei existente (pentru ca a fost deja salvata in 'a')
            evalStiva.push(neg(a)); // se neaga valoarea variabilei (cea care a fost in varful stivei si a fost salvata in a), iar rezultatul negatiei este introdus inapoi in stiva de evaluare
        } else { // daca elementul urmator din forma postfixata este oricare alt operator logic in afara de 'n' - negatie
            int b = evalStiva.top(); evalStiva.pop(); // se salveaza valoarea variabilei din varful stivei - 1st, in b (daca a+b, in forma post fixata vom avea ab+, dar in stiva de evaluare a formulei vom avea b, a)
            int a = evalStiva.top(); evalStiva.pop(); // se salveaza valoarea celei de a doua variabile din varful stivei - 2nd, in a
            if (token == '&') evalStiva.push(conj(a, b)); // daca operatorul din forma postfixata este &, se calculeaza conjunctia intre a si b, in ordinea asta
            else if (token == 'V') evalStiva.push(disj(a, b)); // daca operatorul din forma postfixata este v, se calculeaza disjunctia
            else if (token == '-') evalStiva.push(impl(a, b)); // daca operatorul din forma postfixata este -, se calculeaza implicatia
            else if (token == '~') evalStiva.push(echiv(a, b)); // daca operatorul din forma postfixata este ~, se calculeaza echivalenta
        }
    }

    return evalStiva.top(); // rezultatul final se afla in varful stivei
}

void afisareTabelAdevar(const std::string& bitString) {
    int numVars = log2(bitString.length()); // ia lungimea sirului si da numarul de variabile - 16 bits - 4^2 - 4 var

    std::vector<char> varList = {'p', 'q', 'r', 's', 't'};  // maxim de 5 variabile, se pot adauga mai multe daca vrem

    // ajusteaza stringul de variabile la numarul de variabile pe care il avem
    varList.resize(numVars);

    int nrRanduri = bitString.length();  // nr randuri = lungime sir
    /*
    int nrRanduri = 1 << numVars; // se pot shifta bitii lui 1 cu 4 pozitii la stanga 1 ~= 0001 binar, numVars = 4 -> 16 ~= 10000 binar
    sau 00001 binar, numVars = 5 -> 5 pozitii shiftat -> 100000 binar ~= 32 randuri
    */

    // capete de tabel
    for (char var : varList) {
        std::cout << var << " ";
    }
    std::cout << " FF" << std::endl; // FF coloana cu rezultatele functiei

    std::cout << "----------" << std::endl;


    for (int i = 0; i < nrRanduri; ++i) {
        std::unordered_map<char, int> valVar;  // se declara o mapa neordonata care asociaza fiecarei variabile distincte char (A,B,..) o variabila int pentru randul curent din tabel


        for (int j = 0; j < numVars; ++j) {
            int divizor = 1; // variabila cea mai din stanga isi schimba valoarea la fiecare 2^(n-1) (A la fiecare 8 randuri) randuri, urmatoarea la 2^(n-2) (B la fiecare 4 randuri) ,...
            for (int k = 0; k < (numVars - j - 1); ++k) { // k - contor pentru multiplicarea repetata a divizorului cu 2
                divizor *= 2;  // Calculate the divisor to determine the value of the current variable
            }
            int valBit = (i / divizor) % 2;  // se atribuie o valoare de adevar (0 sau 1) pentru variabila 'j', de pe randul curent 'i'

            valVar[varList[j]] = valBit;  // se salveaza valoarea variabilei j pentru randul i in mapa
        }

        // se afiseaza valorile variabilelor pentru randul i
        for (char var : varList) {
            std::cout << valVar[var] << " ";
        }

        // se afiseaza rezultatul ecuatiei pentru valorile de pe randul i, sau pozitia i in string
        std::cout << " " << bitString[i] << std::endl;
    }
}

// Functia principala ce rezolva ecuatia
int FF(std::string formula, std::unordered_map<char, int> valori) {

    std::string postfix = infixToPostfix(formula); // Converteste formula din forma sa infixata in forma sa postfixata
    //std::cout << "Postfix notation: " << postfix << std::endl;


    int result = evaluarePostfix(postfix, valori); // se evalueaza forma postfixata pentru un anumit set de valori ale variabilelor
    return result;
}

// Functie pentru extragerea variabilelor distincte din formula
std::set<char> getDistinctVariables(const std::string& formula) {
    std::set<char> distinctVars; // declarare set-container pentru retinerea variabilelor distincte
    for (char token : formula) { // se parcurge formula, de la stanga la dreapta, element cu element
        if (verifVar(token) != '0') { // daca elementul curent este variabila
            distinctVars.insert(token); // se introduce acest element in setul de variabile distincte
        }
    }
    return distinctVars;
}

// Afiseaza tabelul de adevar pentru formula
void afisareTabelAdevar2(const std::string& formula) {
    std::set<char> distinctVars = getDistinctVariables(formula); // declarare si initializare set cu variabilele distincte din formula
    int numVars = distinctVars.size(); // numarul de variabile distincte reprezinta dimensiunea setului ce tine variabilele distincte
    std::vector<char> varList(distinctVars.begin(), distinctVars.end()); // declara si initializeaza un vector de tip char, copiind toate elementele setului de variabile distincte distinctVars

    // nr total de randuri pentru tabelul de adevar este 2^n (n - nr de variabile)
    int nrRanduri = 1;
    for (int i = 0; i < numVars; ++i) {
    nrRanduri *= 2; // 2^n numar de randuri sau 2^numVars
    }
    // se poate inlocui forul cu totalRows = pow(2, numVars);  -   #include <cmath>
    // se afiseaza capul tabelului
    for (char var : varList) { // se citesc variabilele din vectorul de variabile atata timp cat avem variabile de citit
        std::cout << var << " "; // se afiseaza variabilele pentru capul de tabel
    }
    std::cout << " FF" << std::endl; // dupa ce se termina variabilele, afisam si capul coloanei pentru rezultatul functiei
    std::cout <<"----------" << std::endl;
    for (int i = 0; i < nrRanduri; i++) { // se trece prin toate randurile de tabel

        std::unordered_map<char, int> valVar; // se declara o mapa neordonata care asociaza fiecarei variabile distincte char (A,B,..) o variabila int pentru randul curent din tabel
        for (int j = 0; j < numVars; j++) {
            int divizor = 1; // variabila cea mai din stanga isi schimba valoarea la fiecare 2^(n-1) (A la fiecare 8 randuri) randuri, urmatoarea la 2^(n-2) (B la fiecare 4 randuri) ,...
            for (int k = 0; k < (numVars - j - 1); k++) { // k - contor pentru multiplicarea repetata a divizorului cu 2
                divizor *= 2;
        } // == divizor=pow(2,numVars-j-1)
        int valBit = (i / divizor) % 2; // se atribuie o valoare de adevar (0 sau 1) pentru variabila 'j', de pe randul curent 'i'

        valVar[varList[j]] = valBit; // se salveaza valoarea variabilei j pentru randul i in mapa
    }

        // se calculeaza ecuatia pentru valorile de pe randul i
        int result = FF(formula, valVar);

        // se afiseaza valorile variabilelor pentru randul i
        for (char var : varList) {
            std::cout << valVar[var] << " ";
        }

        std::cout <<" "<< result << std::endl; // se afiseaza rezultatul ecuatiei pentru valorile de pe randul i
    }
}

int main() {
    std::string bitString = "0111110101011110"; // "0111110101011110" 4 variabile, sau "00111010100010111101110111000000" pentru 5 variabile - Problema47
    int length = bitString.length();
    std::cout << "Problema 47: " << bitString << std::endl;

    int numVars = std::log2(length);
    std::vector<char> varList = {'p', 'q', 'r', 's', 't'};
    std::vector<std::string> FNCSir;
    std::vector<std::string> FNDSir;

    afisareTabelAdevar(bitString);
    std::cout << std::endl;
    std::cout << "Problema 47: " << bitString << std::endl;
    // pentru fiecare rand din tabelul de adevar
    for (int i = 0; i < length; i++) {
        std::unordered_map<char, int> valVar;

        // se calculeaza valoarea fiecarei variabile in functie de indexul i
        for (int j = 0; j < numVars; j++) {
            int divizor = 1; // variabila cea mai din stanga isi schimba valoarea la fiecare 2^(n-1) (A la fiecare 8 randuri) randuri, urmatoarea la 2^(n-2) (B la fiecare 4 randuri) ,...
            for (int k = 0; k < (numVars - j - 1); k++) { // k - contor pentru multiplicarea repetata a divizorului cu 2
                divizor *= 2;
        } // == divizor=pow(2,numVars-j-1)
        int valBit = (i / divizor) % 2; // se atribuie o valoare de adevar (0 sau 1) pentru variabila 'j', de pe randul curent 'i'


        /*
        pentru randul 4, i=3
        A: divizor = 8
            valoareBit=(3/8)%2=0%2=0 -> A=0
        B: divizor = 4
            valoareBit=(3/4)%2=0%2=0 -> B=0
        C: divizor = 2
            valoareBit=(3/2)%2=1%2=0 -> C=1
        D: divizor = 1
            valoareBit=(3/1)%2=3%2=0 -> D=1
        pentru randul 3, vom avea:
        A B C D
        0 0 1 1
        */

            valVar[varList[j]] = valBit;  // se salveaza valoarea variabilei j pentru randul i in mapa
        }

        // in functie de rezultatul functiei, bitString[i], se calculeaza forma normala conjunctiva sau disjunctiva
        std::string InchDesch;
        if (bitString[i] == '0') { // FNC = conjunctie de disjunctii elementare
            InchDesch = "(";
            for (int j = 0; j < numVars; j++) {
                if (j > 0) InchDesch += "V";
                InchDesch += valVar[varList[j]] == 0 ?  std::string(1, varList[j]) : "n" + std::string(1, varList[j]);
            }			// daca variabila este egala cu 1, se adauga n in fata ei pentru negare si apoi se adauga la sir. altfel, se adauga la sir fara negare
            InchDesch += ")";
            FNCSir.push_back(InchDesch);
        } else { // bitString[i] == FF = 1 -> FND = disjunctie de conjunctii elementare
            InchDesch = "(";
            for (int j = 0; j < numVars; j++) {
                if (j > 0) InchDesch += "&";
                InchDesch += valVar[varList[j]] == 1 ? std::string(1, varList[j]) : "n" + std::string(1, varList[j]);
            }			// daca variabila este egala cu 1 se adauga la sir asa cum este. daca nu, se neaga si apoi se adauga la sir
            InchDesch += ")";
            FNDSir.push_back(InchDesch);
        }
    }

    // Afisare FNC
    std::cout << "FNC:      conjunctie de disjunctii elementare      FF=0\n";
    for (size_t i = 0; i < FNCSir.size(); i++) {
        if (i > 0) std::cout << "&\n";
        std::cout << FNCSir[i];
    }
    std::cout << std::endl;

    // Afisare FND
    std::cout << "FND:      disjunctie de conjunctii elementare      FF=1\n";
    for (size_t i = 0; i < FNDSir.size(); i++) {
        if (i > 0) std::cout << "V\n";
        std::cout << FNDSir[i];
    }
    std::cout << std::endl << std::endl;
    std::cout <<"Verificare FNC"<<std::endl;

    std::string formula;
    for (const auto& str : FNCSir) {
    formula += str+'&';
    }
    if (!formula.empty()) { // eliminam ultimul &
    formula.pop_back();
    }
    std::cout << formula << std::endl;
    afisareTabelAdevar2(formula);

    formula = "";
    std::cout <<"Verificare FND"<<std::endl;
    for (const auto& str : FNDSir) {
    formula += str+'V';
    }
    if (!formula.empty()) { // eliminam ultimul V
    formula.pop_back();
    }

    std::cout << formula << std::endl;
    afisareTabelAdevar2(formula);
    return 0;
}
