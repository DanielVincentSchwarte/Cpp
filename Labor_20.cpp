#include <iostream>
#include <string>

using namespace std;


class AuthenticationResult {
public:
    const bool _c_is_authenticated;
    const string _c_username;

    AuthenticationResult(bool is_authenticated, string username = "unauthenticated_user")
        : _c_is_authenticated(is_authenticated),
        _c_username(username){}

    bool get_is_authenticated() const{
        return _c_is_authenticated;
    }

    string get_username() const {
        return _c_username;
    }
};

class IAuthenticationProcedure {
public:
    virtual AuthenticationResult authenticate() = 0;
};

class MockAuth : public IAuthenticationProcedure {
public :
    AuthenticationResult authenticate()override {
        // Immer Authentifiziert
        return AuthenticationResult(true, "Default");
    }
};

class UsernamePassword : public IAuthenticationProcedure {
public:
    AuthenticationResult authenticate()override {
        string username, password;
        cout << "Username:" << endl;
        cin >> username;
        cout << "Password:" << endl;
        cin >> password;

        if (username == password) {
            return AuthenticationResult(true, username);
        }
        else {
            return AuthenticationResult(false);
        }

    }
};

class Certificate : public IAuthenticationProcedure {
    AuthenticationResult authenticate() override {
        string certificate_issuer;
        
        cout << "Zertifikatsaussteller:" << endl;
        cin >> certificate_issuer;

        if (certificate_issuer == "hs-esslingen") {
            return AuthenticationResult(true, "certificate.owner");
        }
        else {
            return AuthenticationResult(false);
        }
    }
};

class Client   {
    IAuthenticationProcedure* _authentication_procedure;
public:
    void set_authentication_procedure(IAuthenticationProcedure* auth_proc) {
        _authentication_procedure = auth_proc;
    }
    void execute() {
        AuthenticationResult result = _authentication_procedure->authenticate();

        if (result.get_is_authenticated()) {
            cout << "Das Programm wird ausgeführt für " << result.get_username() << "." << endl;
        }
        else {
            cout << "Das Programm konnte nicht ausgeführt werden." << endl;
        }
    }

};


int main(int argc, char* argv[]) {
    Client client;

    cout << "Authentifizierung über das Authentifizierungsverfahren Mock Authentication" << endl;
    IAuthenticationProcedure* mock_authentication = new MockAuth();
    client.set_authentication_procedure(mock_authentication);
    client.execute();

    cout << "Authentifizierung über das Authentifizierungsverfahren Username Password" << endl;
    IAuthenticationProcedure* username_password = new UsernamePassword();
    client.set_authentication_procedure(username_password);
    client.execute();

    cout << "Authentifizierung über das Authentifizierungsverfahren Zertifikat" << endl;
    IAuthenticationProcedure* certificate = new Certificate();
    client.set_authentication_procedure(certificate);
    client.execute();
    return 0;
}

