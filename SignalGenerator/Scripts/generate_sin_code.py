
import math
import matplotlib.pyplot as plt

widths = 8       # 正弦数据位数
lengths =256     # 采样点数
code = []        # 输出16进制整形

for x in range(lengths):
    data = round((2 ** (widths - 1) - 1) * math.sin((2 * math.pi / lengths) * x) + (2 ** (widths - 1)),0)
    intdata = int(data)
    code.append(intdata)

for cnt, data in zip(range(lengths), code):
    cnt = cnt + 1
    print(data, end=", ")
    if cnt % 12 == 0 :
        print("\n")

plt.plot(code)
plt.show()
