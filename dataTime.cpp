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
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace boost::gregorian;
using namespace std;

int main()
{
    //(1) return current time.
    string strTime = boost::posix_time::to_iso_string(\
    boost::posix_time::second_clock::local_time() );

    int pos = strTime.find('T');
    strTime.replace(pos,1,string("-"));
    strTime.replace(pos + 3,0,string(":"));
    strTime.replace(pos + 6,0,string(":"));

    cout << strTime << endl;                //: 20180526-15:08:24

    //(2) use date time data type.
    date dt(2018, 5, 26);
    assert( !dt.is_not_a_date() );          // If it is not a date, assert
    cout << dt.week_number() << endl;       //: 21

    // assert( !date().is_not_a_date() );   //: assert

    //(3) date calculation
    months m( 6 );
    years y( 1 );
    months m2 = y + m;
    cout << m2.number_of_months() << endl;  //: 18

    date d1( 2018, 5, 26 );
    date d2( 2018, 1, 1 );
    cout << d1 - d2 << endl;                //: 145

    date_duration ddr( 100 );
    cout << d1 + ddr << endl;               //: 2018-Sep-03
    return 0;
}
