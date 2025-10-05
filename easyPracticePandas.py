import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

data_1 = pd.read_csv("./ReportCard1.txt", sep = '\s+')###这个comma我真的哭死
data_2 = pd.read_csv("./ReportCard2.txt", sep = '\s+')
merged_data = pd.merge(data_1, data_2, on= 'id', how = 'outer')
merged_data = merged_data.sort_values(by = 'id')
merged_data = merged_data.reset_index(drop = True)

score_columns = merged_data.columns.drop('id','sex')
average_scores = merged_data[score_columns].mean(axis = 1)
merged_data['average_scores'] = average_scores
merged_data['sum_scores'] = merged_data[score_columns].sum(axis = 1)
merged_data = merged_data.sort_values(by = 'sum_scores', ascending = False)
gender_scores_avg = merged_data.groupby('sex')[score_columns].mean()
print("avg_scores by sex:\n", gender_scores_avg,"\n")
rate = [0, 60, 70, 80, 90, 101]
labels = ['不及格', '及格', '中', '良', '优']
merged_data['rate'] = pd.cut(merged_data['average_scores'],bins = rate,labels = labels, right = False) #注意使用关键字传递参数
gender_rate = merged_data.groupby('sex')['rate'].value_counts()
print("rate counts by sex: \n",gender_rate)


####直方图
plt.figure(figsize = (8,8))
plt.title('hist')
plt.hist(merged_data['sum_scores'], bins = 10)  # bins 会划分出十个等间距
plt.xlabel("sum_score")
plt.ylabel("num")
plt.show()
###饼状图
plt.figure(figsize = (9,9))
rate_counts = merged_data['rate'].value_counts()
plt.pie(rate_counts, labels = rate_counts.index)
plt.show()
##散点图
plt.figure(figsize = (8,8))
plt.scatter(merged_data['math'], merged_data['average_scores'], )
plt.title("relationship")
plt.xlabel("math")
plt.ylabel("avg_scores")
plt.show()





# 2、Python编程：学生成绩数据的图形化展示。
##对以上两份数据文件，做如下图形化展示：
#1）绘制总成绩的直方图；
#2）绘制平均成绩的优、良、中、及格和不及格的饼图；
#3）绘制总成绩和数学（math）成绩的散点图