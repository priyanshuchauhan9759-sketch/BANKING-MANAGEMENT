#include <iostream>
#include <fstream>
#include <iomanip>
#include <cctype>
using namespace std;

class Bank {
    unsigned long long accno;   // 12-digit account number
    char name[51], type;        // name + account type
    int balance;                // account balance
public:
    void Get_Data();
    void Show_account() const;
    void report() const;
    void dep(int x) { balance += x; }
    void draw(int x) { balance -= x; }
    unsigned long long retacno() const { return accno; }
    int retdeposit() const { return balance; }
    char rettype() const { return type; }
    void Modification_data();
    void Write_Data();
    void Display_Record(unsigned long long);
};

void Bank::Get_Data() {
    cout << "\nEnter Account Number (12 digits): "; cin >> accno;
    while (to_string(accno).size()!=12) { cout << "Try again: "; cin >> accno; }
    cout << "Enter Name: "; cin.ignore(); cin.getline(name,51);
    cout << "Type [S/C]: "; cin >> type; type = toupper(type);
    while (type!='S' && type!='C') { cout << "Enter S or C: "; cin >> type; type=toupper(type); }
    cout << "Initial Deposit (>=500 for S, >=1000 for C): "; cin >> balance;
    cout << "Account Created.\n";
}

void Bank::Show_account() const {
    cout << "\nAcc No: " << accno << "\nName: " << name
         << "\nType: " << (type=='S'?"Saving":"Current")
         << "\nBalance: " << balance << endl;
}

void Bank::Modification_data() {
    cout << "Modify Name: "; cin.ignore(); cin.getline(name,51);
    cout << "Modify Type (S/C): "; cin >> type; type=toupper(type);
    cout << "Modify Balance: "; cin >> balance;
}

void Bank::report() const {
    cout << setw(12) << accno << "  " << setw(20) << name
         << "  " << setw(6) << type << "  " << setw(10) << balance << endl;
}

void Bank::Write_Data() {
    ofstream out("account.dat", ios::binary|ios::app);
    Get_Data(); out.write((char*)this,sizeof(*this));
}

void Bank::Display_Record(unsigned long long n) {
    ifstream in("account.dat", ios::binary); bool found=false;
    while (in.read((char*)this,sizeof(*this)))
        if (retacno()==n) { Show_account(); found=true; }
    if (!found) cout << "Account not found.\n";
}

void Get_All() {
    Bank b; ifstream in("account.dat", ios::binary);
    cout << "\nA/C No        Name                Type   Balance\n";
    while (in.read((char*)&b,sizeof(Bank))) b.report();
}

void Deposit_Withdraw(unsigned long long n,int opt) {
    Bank b; fstream f("account.dat",ios::binary|ios::in|ios::out);
    bool found=false; int amt;
    while (f.read((char*)&b,sizeof(Bank)) && !found) {
        if (b.retacno()==n) {
            b.Show_account(); cout<<(opt==1?"Deposit Amt: ":"Withdraw Amt: "); cin>>amt;
            if (opt==1) b.dep(amt);
            else if ((b.retdeposit()-amt<(b.rettype()=='S'?500:1000))) cout<<"Insufficient balance.\n";
            else b.draw(amt);
            int pos=-sizeof(Bank); f.seekp(pos,ios::cur); f.write((char*)&b,sizeof(Bank));
            cout<<"Updated.\n"; found=true;
        }
    }
    if (!found) cout<<"Record not found.\n";
}

void Delete_Account(unsigned long long n) {
    Bank b; ifstream in("account.dat",ios::binary);
    ofstream out("temp.dat",ios::binary);
    while (in.read((char*)&b,sizeof(Bank)))
        if (b.retacno()!=n) out.write((char*)&b,sizeof(Bank));
    in.close(); out.close(); remove("account.dat"); rename("temp.dat","account.dat");
    cout<<"Account Deleted.\n";
}

void Modify(unsigned long long n) {
    Bank b; fstream f("account.dat",ios::binary|ios::in|ios::out); bool found=false;
    while (f.read((char*)&b,sizeof(Bank)) && !found) {
        if (b.retacno()==n) { b.Show_account(); b.Modification_data();
            int pos=-sizeof(Bank); f.seekp(pos,ios::cur); f.write((char*)&b,sizeof(Bank));
            cout<<"Updated.\n"; found=true; }
    }
    if (!found) cout<<"Record not found.\n";
}

int main() {
    char ch; unsigned long long n; Bank b;
    do {
        cout << "\n1.Create\n2.Modify\n3.Balance Enquiry\n4.Deposit\n5.Withdraw\n6.All Accounts\n7.Delete\n8.Exit\nChoice: ";
        cin >> ch;
        switch(ch) {
            case '1': b.Write_Data(); break;
            case '2': cout<<"Acc No: "; cin>>n; Modify(n); break;
            case '3': cout<<"Acc No: "; cin>>n; b.Display_Record(n); break;
            case '4': cout<<"Acc No: "; cin>>n; Deposit_Withdraw(n,1); break;
            case '5': cout<<"Acc No: "; cin>>n; Deposit_Withdraw(n,2); break;
            case '6': Get_All(); break;
            case '7': cout<<"Acc No: "; cin>>n; Delete_Account(n); break;
            case '8': cout<<"Goodbye!\n"; break;
            default: cout<<"Invalid.\n";
        }
    } while(ch!='8');
}
