#include <iostream>
#include <string>
#include <list>
#include <chrono>
#include <fstream>

using namespace std;

class Suc {
private:
    string nume;
    string fructProvenienta;
    bool cuZahar;
    double cantitateZahar;
    string indulcitor;

public:
    Suc(string nume, string fructProvenienta, bool cuZahar, double cantitateZahar, string indulcitor) {
        this->nume = nume;
        this->fructProvenienta = fructProvenienta;
        this->cuZahar = cuZahar;
        this->cantitateZahar = cantitateZahar;
        this->indulcitor = indulcitor;
    }
        
    string getNume() const {
        return this->nume;
    }

    string getFruct() const {
        return this->fructProvenienta;
    }

    bool isCuZahar() const {
        return this->cuZahar;
    }

    double getCantitateZahar() const {
        return this->cantitateZahar;
    }

    string getIndulcitor() const {
        return this->indulcitor;
    }
};

class DepozitSucuri {
private:
    list<Suc> sucuriDisponibile;
    int capacitateMaxima;

public:
    DepozitSucuri(int capacitateMaxima) {
        this->capacitateMaxima = capacitateMaxima;
    }

    void adaugareSucuri(Suc suc) {
        sucuriDisponibile.push_back(suc);
    }

    void afisareSucuri() const {
        for (const auto& suc : sucuriDisponibile) {
            cout << "Nume: " << suc.getNume() << endl;
            cout << "Fruct: " << suc.getFruct() << endl;

            if (suc.isCuZahar()) {
                cout << "Cu zahar" << endl;
                cout << "Cantitate Zahar: " << suc.getCantitateZahar() << " grame" << endl;
            }
            else {
                cout << "Fara zahar" << endl;
            }

            cout << "Indulcitor: " << suc.getIndulcitor() << endl;
            cout << "------------------------" << endl;
        }
    }

    bool verificaDisponibilitate(const string& numeSuc) const {
        for (const auto& suc : sucuriDisponibile) {
            if (suc.getNume() == numeSuc) {
                return true;
            }
        }
        return false;
    }

    void incarcaSucuriDinFisier() {
        ifstream fisierSucuri("sucuri.txt");

        if (!fisierSucuri.is_open()) {
            cerr << "Eroare la deschiderea fisierului pentru citire!" << endl;
            return;
        }

        string nume, fructProvenienta, cuZaharStr, cantitateZaharStr, indulcitor;
        while (getline(fisierSucuri, nume, ',') && getline(fisierSucuri, fructProvenienta, ',') &&
            getline(fisierSucuri, cuZaharStr, ',') && getline(fisierSucuri, cantitateZaharStr, ',') &&
            getline(fisierSucuri, indulcitor)) {
            bool cuZahar = (cuZaharStr == "1");
            double cantitateZahar = stod(cantitateZaharStr);
            sucuriDisponibile.push_back(Suc(nume, fructProvenienta, cuZahar, cantitateZahar, indulcitor));
        }

        fisierSucuri.close();
        cout << "Sucurile au fost incarcate cu succes din fisierul sucuri.txt!" << endl;
    }

    Suc preparareSuc(const string& numeSuc) {
        string fructProvenienta;
        bool cuZahar = false;
        double cantitateZahar = 0.0;
        string indulcitor;

        bool sucExista = false;
        for (const auto& suc : sucuriDisponibile) {
            if (suc.getNume() == numeSuc) {
                sucExista = true;

                fructProvenienta = suc.getFruct();
                cuZahar = suc.isCuZahar();
                cantitateZahar = suc.getCantitateZahar();
                indulcitor = suc.getIndulcitor();

                break;
            }
        }

        if (sucExista) {
            cout << "Se prepara sucul " << numeSuc << ". Asteptati..." << endl;

            cout << "Sucul " << numeSuc << " este pregatit!" << endl;
            return Suc(numeSuc, fructProvenienta, cuZahar, cantitateZahar, indulcitor);
        }
        else {
            cout << "Ne pare rau, dar sucul " << numeSuc << " nu este disponibil momentan." << endl;
            return Suc("", "", false, 0.0, "");
        }
    }
};

struct Comanda {
    string numeSuc;
    Suc suc;
};

class Client {
private:
    string nume;
    string adresaEmail;
    string parola;
    list<Comanda> istoricComenzi;

public:
    Client(string nume, string adresaEmail, string parola) {
        this->nume = nume;
        this->adresaEmail = adresaEmail;
        this->parola = parola;
    }

    void setParola(const string& parolaNoua) {
        parola = parolaNoua;
    }

    string getNume() const {
        return this->nume;
    }

    string getAdresaEmail() const {
        return this->adresaEmail;
    }

    string getParola() const {
        return this->parola;
    }

    void adaugaComanda(const string& numeSuc, const Suc& suc) {
        istoricComenzi.push_back({ numeSuc, suc });
    }

    void afiseazaIstoricComenzi() const {
        cout << "Istoric comenzi pentru " << nume << ": " << endl;
        for (const auto& comanda : istoricComenzi) {
            cout << "Suc: " << comanda.numeSuc << endl;
            cout << "Detalii suc: " << endl;
            cout << "  Fruct: " << comanda.suc.getFruct() << endl;
            cout << "  Cu zahar: " << (comanda.suc.isCuZahar() ? "Da" : "Nu") << endl;
            cout << "  Cantitate zahar: " << comanda.suc.getCantitateZahar() << endl;
            cout << "  Indulcitor: " << comanda.suc.getIndulcitor() << endl;
            cout << "----------------------------------" << endl;
        }
    }
};

class JuiceStation {
private:
    list<Client> listaClienti;
    DepozitSucuri depozitSucuri;

public:
    JuiceStation() : depozitSucuri(100) {
        depozitSucuri.incarcaSucuriDinFisier();
        incarcaClientiDinFisier();
    }

    void inregistrareClienti();
    Client* autentificareClient();
    void resetareParolaClient(Client& client);
    void modificareParolaClient(Client& client);

    void afisareSucuriDisponibile() const;
    void afisareSucuriDepozit() const;

    void comandaSuc(Client& client);
    void afisareIstoricComenzi(Client& client);

    bool existaClientCuEmail(const string& email) const;

    void salveazaClientiInFisier() const;
    void incarcaClientiDinFisier();
};

void JuiceStation::inregistrareClienti() {
    string nume, email, parola;
    cout << "Introduceti nume: ";
    cin >> nume;
    cout << "Introduceti adresa de email: ";
    cin >> email;
    if (existaClientCuEmail(email)) {
        cout << "Adresa de email este deja inregistrata! Introduceti alta adresa." << endl;
        return;
    }
    cout << "Introduceti parola: ";
    cin >> parola;
    listaClienti.push_back(Client(nume, email, parola));
    cout << "Inregistrare reusita!" << endl;

    salveazaClientiInFisier();
}

Client* JuiceStation::autentificareClient() {
    string email, parola;
    cout << "Introduceti adresa de email: ";
    cin >> email;
    cout << "Introduceti parola: ";
    cin >> parola;
    for (auto& client : listaClienti) {
        if (client.getAdresaEmail() == email && client.getParola() == parola) {
            cout << "Autentificare reusita!" << std::endl;
            return &client;
        }
    }
    cout << "Autentificare esuata! Verificati adresa de email sau parola." << std::endl;
    return nullptr;
}

void JuiceStation::resetareParolaClient(Client& client) {
    srand(static_cast<unsigned int>(time(nullptr)));
    const string caracterePosibile = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    const int lungimeParola = 8;

    string nouaParola;
    for (int i = 0; i < lungimeParola; ++i) {
        nouaParola += caracterePosibile[rand() % caracterePosibile.length()];
    }

    client.setParola(nouaParola);

    cout << "Parola pentru contul " << client.getAdresaEmail() << " a fost resetata cu succes. Noua parola: " << nouaParola << endl;

    salveazaClientiInFisier();
}

void JuiceStation::modificareParolaClient(Client& client) {
    string parolaAcum, parolaNoua;
    cout << "Introduceti parola curenta: ";
    cin >> parolaAcum;
    if (parolaAcum == client.getParola()) {
        cout << "Introduceti noua parola: ";
        cin >> parolaNoua;
        client.setParola(parolaNoua);
        cout << "Parola modificata cu succes!" << endl;
    }
    else {
        cout << "Parola gresita!" << endl;
    }

    salveazaClientiInFisier();
}

void JuiceStation::afisareSucuriDisponibile() const {
    depozitSucuri.afisareSucuri();
}

void JuiceStation::afisareSucuriDepozit() const {
    depozitSucuri.afisareSucuri();
}

void JuiceStation::comandaSuc(Client& client) {
    string numeSuc;
    cout << "Introduceti numele sucului dorit: ";
    cin >> numeSuc;

    if (depozitSucuri.verificaDisponibilitate(numeSuc)) {
        Suc sucComandat = depozitSucuri.preparareSuc(numeSuc);
        client.adaugaComanda(numeSuc, sucComandat);
        cout << "Comanda plasata cu succes!" << endl;
    }
    else {
        cout << "Sucul nu este disponibil momentan." << endl;
    }
}

void JuiceStation::afisareIstoricComenzi(Client& client) {
    client.afiseazaIstoricComenzi();
}

bool JuiceStation::existaClientCuEmail(const string& email) const {
    for (const auto& client : listaClienti) {
        if (client.getAdresaEmail() == email) {
            return true;
        }
    }
    return false;
}

void JuiceStation::salveazaClientiInFisier() const {
    ofstream fisierClienti("clienti.txt");

    if (!fisierClienti.is_open()) {
        cerr << "Eroare la deschiderea fisierului pentru scriere!" << endl;
        return;
    }

    for (const auto& client : listaClienti) {
        fisierClienti << client.getNume() << "," << client.getAdresaEmail() << "," << client.getParola() << endl;
    }

    fisierClienti.close();
    cout << "Clientii au fost salvati cu succes in fisierul clienti.txt!" << endl;
}

void JuiceStation::incarcaClientiDinFisier() {
    ifstream fisierClienti("clienti.txt");

    if (!fisierClienti.is_open()) {
        cerr << "Eroare la deschiderea fisierului pentru citire!" << endl;
        return;
    }

    string nume, email, parola;
    while (getline(fisierClienti, nume, ',') && getline(fisierClienti, email, ',') && getline(fisierClienti, parola)) {
        listaClienti.push_back(Client(nume, email, parola));
    }

    fisierClienti.close();
    cout << "Clientii au fost incarcati cu succes din fisierul clienti.txt!" << endl;
}

int main() {
    JuiceStation juiceStation;

    while (true) {
        cout << "Meniu:" << endl;
        cout << "1. Inregistrare client nou" << endl;
        cout << "2. Autentificare client existent" << endl;
        cout << "3. Afisare sucuri disponibile" << endl;
        cout << "4. Afisare sucuri existente in depozit" << endl;
        cout << "5. Comanda suc" << endl;
        cout << "6. Afisare istoric comenzi" << endl;
        cout << "7. Iesire" << endl;

        int optiune;
        cout << "Alegeti o optiune: ";
        cin >> optiune;

        switch (optiune) {
        case 1:
            juiceStation.inregistrareClienti();
            break;
        case 2: {
            Client* clientAutentificat = juiceStation.autentificareClient();
            if (clientAutentificat != nullptr) {
                int suboptiune;
                do {
                    cout << "Submeniu:" << endl;
                    cout << "1. Resetare parola" << endl;
                    cout << "2. Modificare parola" << endl;
                    cout << "3. Afisare sucuri disponibile" << endl;
                    cout << "4. Comanda suc" << endl;
                    cout << "5. Afisare istoric comenzi" << endl;
                    cout << "6. Deconectare" << endl;

                    cout << "Alegeti o suboptiune: ";
                    cin >> suboptiune;

                    switch (suboptiune) {
                    case 1:
                        juiceStation.resetareParolaClient(*clientAutentificat);
                        break;
                    case 2:
                        juiceStation.modificareParolaClient(*clientAutentificat);
                        break;
                    case 3:
                        juiceStation.afisareSucuriDisponibile();
                        break;
                    case 4:
                        juiceStation.comandaSuc(*clientAutentificat);
                        break;
                    case 5:
                        juiceStation.afisareIstoricComenzi(*clientAutentificat);
                        break;
                    case 6:
                        cout << "Deconectare..." << endl;
                        break;
                    default:
                        cout << "Optiune invalida!" << endl;
                    }
                } while (suboptiune != 6);
            }
            break;
        }
        case 3:
            juiceStation.afisareSucuriDisponibile();
            break;
        case 4:
            juiceStation.afisareSucuriDepozit();
            break;
        case 5: {
            Client* clientAutentificat = juiceStation.autentificareClient();
            if (clientAutentificat != nullptr) {
                juiceStation.comandaSuc(*clientAutentificat);
            }
            break;
        }
        case 6: {
            Client* clientAutentificat = juiceStation.autentificareClient();
            if (clientAutentificat != nullptr) {
                juiceStation.afisareIstoricComenzi(*clientAutentificat);
            }
            break;
        }
        case 7:
            cout << "Iesire..." << endl;
            return 0;
        default:
            cout << "Optiune invalida!" << endl;
        }
    }

    return 0;
}