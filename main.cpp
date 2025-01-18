#include "gui/L2/Transaction.hpp"
#include <iostream>
#include <string>

int main(){
    std::string str2 = "TXN-1,USER-97,Apple pay,Random transaction details,01-03-2023,870.018,97.2177,3.3771,5,1,6911761180845884,343,0,CNY";
    std::string str1 = "TXN-0,USER-390,MIR pay,Random transaction details,30-10-2023,810.428,19.6255,3.88737,4,2,9781936969081869,335,1,CAD";
    std::cout << to_string(from_string(str2)) << std::endl;
}