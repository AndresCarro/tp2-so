#include <dates.h>

static uint8_t getPreviousDay(uint8_t day);


uint8_t localHours(void)
{
    uint8_t hour = getHours();
    if (hour >= 0 && hour <= 3)
    {
        return hour - 3 + 21;
    }
    return hour - 3;
}

static uint8_t getPreviousDay(uint8_t day){
    uint8_t year = getYear();
    uint8_t dayMonth[13] = { 31, 31 , 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 0};
    if( year%4==0 && ( year%100!=0 || year%400==0 )){
        dayMonth[2]=29;
    }
    return dayMonth[ (int)localMonth()-1 ];
}

uint8_t localDay(void)
{
    uint8_t day = getDay();
    if(getHours()<=3){
        if(day==1){
            getPreviousDay(day);
        }
        return day-1;
    }
    return getDay();
}

uint8_t localMonth(void)
{
    if( getHours()<=3 && getDay()==1 ){
        if(getMonth()==1){
            return 12;
        }
        return getMonth()-1;
    }
    return getMonth();
}

uint8_t localYear(void)
{
    if( getHours()<=3 && getDay()==1 && getMonth()==1 ){
        return getYear()-1;
    }
    return getYear();

}