#include "IPC.h"
#include "Util.h"

namespace ipc {
    void IPC::write(const char* data) {
        if (inputFileDesc > -1) {
            if (inputFile != NULL) {
                const int cbData = strlen(data);
                in = std::ofstream(inputFile);

                if (in.is_open()) {
                    in.write(data, cbData);
                    in.close();
                }

                /*DWORD exitCode;
                if (GetExitCodeProcess(piProcInfo.hProcess, &exitCode)) {
                    PrintError(L"Process exited");
                }*/
            }
        }
    }

    void IPC::read() {
        if (outputFileDesc > -1) {
            if (outputFile != NULL) {
                std::string ret;
                char buffer[BUFSIZE];

                while (out.read(buffer, sizeof(buffer)))
                    ret.append(buffer, sizeof(buffer));
                ret.append(buffer, out.gcount());

                printf_s("%s", ret.c_str());
            }
        }
    }

    void IPC::read(std::wstring* ret) {
        if (outputFileDesc > -1) {
            if (outputFile != NULL) {
                std::string re2;

                char buffer[BUFSIZE];
                while (out.read(buffer, sizeof(buffer)))
                    re2.append(buffer, sizeof(buffer));
                re2.append(buffer, out.gcount());

                try {
                    util::string_to_wstring(re2, ret);
                }
                catch (std::exception ex) {
                    error::PrintError(L"Error converting string");
                }
            }
        }
    }

    bool IPC::init_input(HANDLE* hInputWr) {
        if ((inputFileDesc = _open_osfhandle((intptr_t)*hInputWr, 0)) == -1) {
            error::PrintError(L"IPC::init_input->_open_osfhandle");
            return false;
        }

        if ((inputFile = _fdopen(inputFileDesc, "w")) == NULL) {
            error::PrintError(L"IPC::init_input->_fdopen");
            return false;
        }

        in = std::ofstream(inputFile);
        return true;
    }

    bool IPC::init_output(HANDLE* hOutputRd) {
        if ((outputFileDesc = _open_osfhandle((intptr_t)*hOutputRd, 0)) == -1) {
            error::PrintError(L"IPC::init_output->_open_osfhandle");
            return false;
        }


        if ((outputFile = _fdopen(outputFileDesc, "r")) == NULL) {
            error::PrintError(L"IPC::init_output->_fdopen");
            return false;
        }

        out = std::ifstream(outputFile);
        return true;
    }
}