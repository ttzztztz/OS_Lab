# shell for unix / linux only!

inputFunction() {
    echo "Enter birth year(eg: 2020): "
    read year
    if [ "$year" == "q" -o "$year" == "Q" ]
    then
        return 0
    fi
    echo "Enter birth month(eg: 01): "
    read month
    if [ "$month" == "q" -o "$month" == "Q" ]
    then
        return 0
    fi
    echo "Enter birth day(eg: 02)"
    read day
    if [ "$day" == "q" -o "$day" == "Q" ]
    then
        return 0
    fi

    return 1
}

checkValid() {
    if [ $year -lt 0 ]
    then
        echo "Invalid Year"
        return 0
    fi

    if [ $month -le 0 -o $month -ge 13 ]
    then
        echo "Invalid Month"
        return 0
    fi

    MonthArr=(0 31 0 31 30 31 30 31 31 30 31 30 31)
    if [ `expr $year % 400` -eq 0 ]
    then
        MonthArr[2]=29
    elif [ `expr $year % 4` -eq 0 ] && [ `expr $year % 100` -ne 0 ]
    then
        MonthArr[2]=29
    else
        MonthArr[2]=28
    fi

    if [[ $day -le 0 || $day -gt MonthArr[$month] ]]
    then
        echo "Invalid Day"
        return 0
    fi

    return 1
}

process() {
    Year=("鼠🐭" "牛🐂" "虎🐯" "兔🐰" "龙🐲" "蛇🐍" "马🐎" "羊🐑" "猴🐒" "鸡🐔" "狗🐶" "猪🐷")
    yearIndex=$(( (($year-2020)%12+12)%12 ))
    
    echo "年份：${Year[$yearIndex]}"

    if [[ $month -eq 12 && $day -ge 21 ]] || [[ $month -eq 1 && $day -le 18 ]]
    then
        xingzuo="魔蝎座"
    elif [[ $month -eq 1 && $day -ge 19 ]] || [[ $month -eq 2 && $day -le 18 ]]
    then
        xingzuo="水瓶座"
    elif [[ $month -eq 2 && $day -ge 19 ]] || [[ $month -eq 3 && $day -le 20 ]]
    then
        xingzuo="双鱼座"
    elif [[ $month -eq 3 && $day -ge 21 ]] || [[ $month -eq 4 && $day -le 19 ]]
    then
        xingzuo="白羊座"
    elif [[ $month -eq 4 && $day -ge 20 ]] || [[ $month -eq 5 && $day -le 20 ]]
    then
        xingzuo="金牛座"
    elif [[ $month -eq 5 && $day -ge 21 ]] || [[ $month -eq 6 && $day -le 21 ]]
    then
        xingzuo="双子座"
    elif [[ $month -eq 6 && $day -ge 22 ]] || [[ $month -eq 7 && $day -le 22 ]]
    then
        xingzuo="巨蟹座"
    elif [[ $month -eq 7 && $day -ge 23 ]] || [[ $month -eq 8 && $day -le 22 ]]
    then
        xingzuo="狮子座"
    elif [[ $month -eq 8 && $day -ge 24 ]] || [[ $month -eq 9 && $day -le 22 ]]
    then
        xingzuo="处女座"
    elif [[ $month -eq 9 && $day -ge 23 ]] || [[ $month -eq 10 && $day -le 23 ]]
    then
        xingzuo="天平座"
    elif [[ $month -eq 10 && $day -ge 24 ]] || [[ $month -eq 1 && $day -le 22 ]]
    then
        xingzuo="天蝎座"
    else
        xingzuo="射手座"
    fi
    echo "星座：${xingzuo}"
}

while :
do
    inputFunction
    if [ $? -eq 0 ] 
    then
    echo "Terminate signal received"
    break
    fi

    echo "Input ${month}/${day}/${year}"

    checkValid
    if [ $? -eq 0 ]
    then
    echo "Invalid Input detected"
    fi
    
    process
done
