
import sys

arr = input("输入计数值arr :")
psc = input("输入预分频值psc :")

hz = 72000000 / ((int(arr)+1) * (int(psc)+1)) 
ms = 1000 / hz

print(f'定时器频率为{hz}HZ，中断时间为{ms}ms')