/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

namespace BinaryData
{

//================== g2filesinkhelper.ipp ==================
static const unsigned char temp_binary_data_0[] =
"/** ==========================================================================\r\n"
" * 2013 by KjellKod.cc. This is PUBLIC DOMAIN to use at your own risk and comes\r\n"
" * with no warranties. This code is yours to share, use and modify with no\r\n"
" * strings attached and no restrictions or obligations.\r\n"
" * \r\n"
" * For more information see g3log/LICENSE or refer refer to http://unlicense.org\r\n"
" * ============================================================================*/\r\n"
"\r\n"
"#pragma once\r\n"
"\r\n"
"\r\n"
"#include <memory>\r\n"
"#include <string>\r\n"
"#include <algorithm>\r\n"
"#include <iostream>\r\n"
"#include <fstream>\r\n"
"#include <sstream>\r\n"
"#include <string>\r\n"
"\r\n"
"\r\n"
"namespace g2 {\r\n"
"  namespace internal {\r\n"
"    static const std::string file_name_time_formatted = \"%Y%m%d-%H%M%S\";\r\n"
"\r\n"
"    // check for filename validity -  filename should not be part of PATH\r\n"
"    bool isValidFilename(const std::string& prefix_filename) {\r\n"
"      std::string illegal_characters(\"/,|<>:#$%{}()[]\\'\\\"^!?+* \");\r\n"
"      size_t pos = prefix_filename.find_first_of(illegal_characters, 0);\r\n"
"      if (pos != std::string::npos) {\r\n"
"        std::cerr << \"Illegal character [\" << prefix_filename.at(pos) << \"] in logname prefix: \" << \"[\" << prefix_filename << \"]\" << std::endl;\r\n"
"        return false;\r\n"
"      } else if (prefix_filename.empty()) {\r\n"
"        std::cerr << \"Empty filename prefix is not allowed\" << std::endl;\r\n"
"        return false;\r\n"
"      }\r\n"
"\r\n"
"      return true;\r\n"
"    }\r\n"
"\r\n"
"    std::string prefixSanityFix(std::string prefix) {\r\n"
"      prefix.erase(std::remove_if(prefix.begin(), prefix.end(), ::isspace), prefix.end());\r\n"
"      prefix.erase(std::remove(prefix.begin(), prefix.end(), '/'), prefix.end());\r\n"
"      prefix.erase(std::remove(prefix.begin(), prefix.end(), '\\\\'), prefix.end());\r\n"
"      prefix.erase(std::remove(prefix.begin(), prefix.end(), '.'), prefix.end());\r\n"
"      prefix.erase(std::remove(prefix.begin(), prefix.end(), ':'), prefix.end());\r\n"
"      if (!isValidFilename(prefix)) {\r\n"
"        return\r\n"
"        {\r\n"
"        };\r\n"
"      }\r\n"
"      return prefix;\r\n"
"    }\r\n"
"\r\n"
"    std::string pathSanityFix(std::string path, std::string file_name) {\r\n"
"      // Unify the delimeters,. maybe sketchy solution but it seems to work\r\n"
"      // on at least win7 + ubuntu. All bets are off for older windows\r\n"
"      std::replace(path.begin(), path.end(), '\\\\', '/');\r\n"
"\r\n"
"      // clean up in case of multiples\r\n"
"      auto contains_end = [&](std::string & in) -> bool {\r\n"
"        size_t size = in.size();\r\n"
"        if (!size) return false;\r\n"
"        char end = in[size - 1];\r\n"
"        return (end == '/' || end == ' ');\r\n"
"      };\r\n"
"\r\n"
"      while (contains_end(path)) {\r\n"
"        path.erase(path.size() - 1);  \r\n"
"      }\r\n"
"      \r\n"
"      if (!path.empty()) { \r\n"
"        path.insert(path.end(), '/');  \r\n"
"      }\r\n"
"      \r\n"
"      path.insert(path.size(), file_name);\r\n"
"      return path;\r\n"
"    }\r\n"
"\r\n"
"    std::string header() {\r\n"
"      std::ostringstream ss_entry;\r\n"
"      //  Day Month Date Time Year: is written as \"%a %b %d %H:%M:%S %Y\" and formatted output as : Wed Sep 19 08:28:16 2012\r\n"
"      ss_entry << \"\\t\\tg2log created log at: \" << g2::localtime_formatted(g2::systemtime_now(), \"%a %b %d %H:%M:%S %Y\") << \"\\n\";\r\n"
"      ss_entry << \"\\t\\tLOG format: [YYYY/MM/DD hh:mm:ss.uuu* LEVEL FILE:LINE] message\\n\\n\"; \r\n"
"      return ss_entry.str();\r\n"
"    }\r\n"
"\r\n"
"    std::string createLogFileName(const std::string& verified_prefix) {\r\n"
"      std::stringstream oss_name;\r\n"
"      oss_name << verified_prefix << \".g2log.\";\r\n"
"      oss_name << g2::localtime_formatted(g2::systemtime_now(), file_name_time_formatted);\r\n"
"      oss_name << \".log\";\r\n"
"      return oss_name.str();\r\n"
"    }\r\n"
"\r\n"
"    bool openLogFile(const std::string& complete_file_with_path, std::ofstream& outstream) {\r\n"
"      std::ios_base::openmode mode = std::ios_base::out; // for clarity: it's really overkill since it's an ofstream\r\n"
"      mode |= std::ios_base::trunc;\r\n"
"      outstream.open(complete_file_with_path, mode);\r\n"
"      if (!outstream.is_open()) {\r\n"
"        std::ostringstream ss_error;\r\n"
"        ss_error << \"FILE ERROR:  could not open log file:[\" << complete_file_with_path << \"]\";\r\n"
"        ss_error << \"\\n\\t\\t std::ios_base state = \" << outstream.rdstate();\r\n"
"        std::cerr << ss_error.str().c_str() << std::endl;\r\n"
"        outstream.close();\r\n"
"        return false;\r\n"
"      }\r\n"
"      return true;\r\n"
"    }\r\n"
"\r\n"
"    std::unique_ptr<std::ofstream> createLogFile(const std::string& file_with_full_path) {\r\n"
"      std::unique_ptr<std::ofstream> out(new std::ofstream);\r\n"
"      std::ofstream & stream(*(out.get()));\r\n"
"      bool success_with_open_file = openLogFile(file_with_full_path, stream);\r\n"
"      if (false == success_with_open_file) {\r\n"
"        out.release();\r\n"
"      }\r\n"
"      return out;\r\n"
"    }\r\n"
"\r\n"
"\r\n"
"  }\r\n"
"}\r\n";

const char* g2filesinkhelper_ipp = (const char*) temp_binary_data_0;


const char* getNamedResource (const char*, int&) throw();
const char* getNamedResource (const char* resourceNameUTF8, int& numBytes) throw()
{
    unsigned int hash = 0;
    if (resourceNameUTF8 != 0)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0x9df15752:  numBytes = 4666; return g2filesinkhelper_ipp;
        default: break;
    }

    numBytes = 0;
    return 0;
}

const char* namedResourceList[] =
{
    "g2filesinkhelper_ipp"
};

}
