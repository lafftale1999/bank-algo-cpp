#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <chrono>

class BankAccount{
    std::string accountNumber;
    float balance;
public:
    BankAccount(){
    }

    BankAccount(std::string accountNumber, float balance = 0)
    :accountNumber(accountNumber),balance(balance)
    {
    }

    std::string getAccountNumber()
    {
	    return this->accountNumber;
    }
};

//INTERFACE - gränssnitt "standard"
class IAccountStorage {
public:    
    virtual void addAccount(BankAccount account) = 0;
	virtual BankAccount *findAccount(std::string accountNumber) = 0;        
};

class SplitVectorStorage : public IAccountStorage
{
    std::vector<std::vector<BankAccount>> accounts = std::vector<std::vector<BankAccount>>(10);

public:
    void addAccount(BankAccount account)
    {
        if(account.getAccountNumber().length() < 7)
        {
            accounts[0].push_back(account);
        }

        else
        {
            int accountPrefix = account.getAccountNumber()[0] - '0';
            accounts[accountPrefix].push_back(account);
        }
    }

    BankAccount* findAccount(std::string accountNumber)
    {
        if(accountNumber.length() < 7)
        {
            for(BankAccount &account : accounts[0])
            {
                if(accountNumber == account.getAccountNumber()) return &account;
            }
        }

        else if (isdigit(accountNumber[0]))
        {
            int accountPrefix = accountNumber[0] - '0';
            for(BankAccount &account : accounts[accountPrefix])
            {
                if(accountNumber == account.getAccountNumber()) return &account;
            }
        }

        return nullptr;
    }
};

class MapStorage : public IAccountStorage{
    std::map<std::string, BankAccount> accounts;

public:
    void addAccount(BankAccount bankAccount) override
    {
        accounts[bankAccount.getAccountNumber()] = bankAccount;
    }
    BankAccount* findAccount(std::string accountNumber) override
    {
        return &accounts[accountNumber];
    }
};

class VectorAccountStorage: public IAccountStorage{
        std::vector<BankAccount> accounts;
public:
    void addAccount(BankAccount account) override{
        accounts.push_back(account);
    }

    BankAccount *findAccount(std::string accountNumber){
        for(BankAccount &account : accounts){
            if(account.getAccountNumber() == accountNumber ) return &account;                                        
        }
        return nullptr;
    }
};

class Bank
{
private:
	IAccountStorage * accountStorage;
public:
	Bank(IAccountStorage *storage):accountStorage(storage){

    }
	bool addAccount(std::string accountNumber){
        //validate
        //if something (accountNumber) return false
        accountStorage->addAccount(accountNumber);
        return true;
    }
	BankAccount *getAccount(std::string accountNumber){
        return accountStorage->findAccount(accountNumber);
    }
};



int main(int, char**){

    //VectorAccountStorage storage;
    SplitVectorStorage storage;

    //MapAccountStorage storage;
    Bank bank(&storage);

    const int AntalAccounts =  10000000;


    std::string sFirst = ""; 
    std::string sLast = "";
    std::string sNotFound = "notfound";

    std::cout << "INITIALIZE: " << std::endl;
    auto startTime = std::chrono::high_resolution_clock::now();
    for(int i = 0;i < AntalAccounts; i++){
        std::string accountNumber =  std::to_string(i);
        if(i == 0){
            sFirst = accountNumber;
        }
        if(i == AntalAccounts-1){
            sLast = accountNumber;
        }
        bank.addAccount(accountNumber);
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << "INIT Took: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime    - startTime).count() << " milliseconds" << std::endl;

    startTime = std::chrono::high_resolution_clock::now();
    BankAccount *p = bank.getAccount(sFirst);
    endTime = std::chrono::high_resolution_clock::now();
    std::cout << p->getAccountNumber() << " took: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime    - startTime).count() << " milliseconds" << std::endl;

    startTime = std::chrono::high_resolution_clock::now();
    p = bank.getAccount(sLast);
    endTime = std::chrono::high_resolution_clock::now();
    std::cout << p->getAccountNumber() << " took: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime    - startTime).count() << " milliseconds" << std::endl;

    startTime = std::chrono::high_resolution_clock::now();
    p = bank.getAccount(sNotFound);
    endTime = std::chrono::high_resolution_clock::now();
    std::cout << "NOT FOUND" << " took: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime    - startTime).count() << " milliseconds" << std::endl;
}
