import pandas as pd
import matplotlib.pyplot as plt

# 读取文件
file_path = 'f:\\VoltageSample.TXT'
data = pd.read_csv(file_path)

# 将日期列转换为日期时间格式
data['Date'] = pd.to_datetime(data['Date'])

# 提取电压列
V1 = data['V1']
V2 = data['V2']
V3 = data['V3']

# 绘制曲线图
plt.figure(figsize=(10, 6))
plt.plot(data['Date'], V1, label='V1', color='blue')
plt.plot(data['Date'], V2, label='V2', color='green')
plt.plot(data['Date'], V3, label='V3', color='red')

# 显示最大值和最小值
max_v1 = V1.max()
min_v1 = V1.min()
max_v2 = V2.max()
min_v2 = V2.min()
max_v3 = V3.max()
min_v3 = V3.min()

plt.scatter(data['Date'][V1.idxmax()], max_v1, color='blue', marker='o', label=f'Max V1: {max_v1}')
plt.scatter(data['Date'][V1.idxmin()], min_v1, color='blue', marker='x', label=f'Min V1: {min_v1}')
plt.scatter(data['Date'][V2.idxmax()], max_v2, color='green', marker='o', label=f'Max V2: {max_v2}')
plt.scatter(data['Date'][V2.idxmin()], min_v2, color='green', marker='x', label=f'Min V2: {min_v2}')
plt.scatter(data['Date'][V3.idxmax()], max_v3, color='red', marker='o', label=f'Max V3: {max_v3}')
plt.scatter(data['Date'][V3.idxmin()], min_v3, color='red', marker='x', label=f'Min V3: {min_v3}')

plt.xlabel('Date')
plt.ylabel('Vo')
plt.title('V1 V2 V3 to G')
plt.legend()
plt.grid(True)
plt.show()
