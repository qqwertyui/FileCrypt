#include <boost/filesystem.hpp>

#include "FileCrypt.hpp"
#include "FileCryptException.hpp"
#include "Log.hpp"
#include "Version.hpp"
#include "Keygen.hpp"
#include "Cmd.hpp"


int main(int argc, char **argv) {
    gflags::SetUsageMessage("Usage: fcrypt.exe --mode <mode> --path <path> --key <path>");
    gflags::SetVersionString(Version::STRING);
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    if(FLAGS_mode.empty() == true) {
        Log::error(gflags::ProgramUsage());
        return Status::INVALID_ARGS;
    } else if(FLAGS_mode.compare("generate") == 0) {
        if(FLAGS_path.empty() == true) {
            Log::error("You need to pass --path\n");
            return Status::INVALID_ARGS;
        }
        try {
            Keygen::generate(FLAGS_path);
        } catch(FileCryptException &e) {
            Log::errorf("Key generation failed: %s\n", e.what());
            return e.get_code();
        }
        boost::filesystem::path full_path(boost::filesystem::current_path());
        Log::infof("Key was written at %s\\%s\n", full_path.string().c_str(), FLAGS_path.c_str());
    } else {
        FileCrypt::Operation operation;
        if(FLAGS_mode.compare("encrypt") == 0) {
            operation = FileCrypt::Encrypt;
        } else if(FLAGS_mode.compare("decrypt") == 0) {
            operation = FileCrypt::Decrypt;
        } else {
            Log::error("Invalid mode\n");
            return Status::INVALID_ARGS;
        }

        if(FLAGS_key.empty() == true) {
            Log::error("You need to pass --key\n");
            return Status::INVALID_ARGS;
        }

        try {
            FileCrypt core(FLAGS_key);
            core.crypt(FLAGS_path, operation);
            Log::info("Succesfully modified files\n");
        } catch(FileCryptException &e) {
            Log::errorf("Errors ocurred: %s\n", e.what());
            return e.get_code();
        }
    }
    return Status::OK;
}