/**********************************************************************************************
*
*      Filename: dataTime.cpp
*
*        Author: WeiStephen - wei_yang1994@163.com
*
*        Create: 2018-05-26 11:01:00
**********************************************************************************************/

#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>

int main()
{
    std::string strTime = boost::posix_time::to_iso_string(\
    boost::posix_time::second_clock::local_time() );

    int pos = strTime.find('T');
    strTime.replace(pos,1,std::string("-"));
    strTime.replace(pos + 3,0,std::string(":"));
    strTime.replace(pos + 6,0,std::string(":"));

    std::cout << strTime << std::endl;
    return 0;
}
