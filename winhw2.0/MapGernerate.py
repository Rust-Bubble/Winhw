import random
import csv
import os
#生成地图点，size为点的数量
def GeneratePointMap(size,filepath):
    res = []
    type = ['DORMITORY','DORMITORY','DORMITORY','TEACHING_BUILDING','TEACHING_BUILDING','TEACHING_BUILDING','EXIT']
    id = 1
    for i in range(size):
        element = []
        element.append(id)
        id += 1
        e_type = random.choice(type)
        if e_type == 'DORMITORY':
            e_name = str(random.randint(100,200)) + '宿舍'
        elif e_type == 'TEACHING_BUILDING':
            e_name = str(random.randint(100,200)) + '教学楼'
        else:
            e_name = '出口'
        e_position = str(random.randint(0,10)) + ' ' + str(random.randint(0,10))
        e_isburning = random.choice([0,0,0,1]) if e_type != 'EXIT' else 0
        element.append(e_name)
        element.append(e_type)
        element.append(e_position)
        element.append(e_isburning)
        res.append(element)
    header = ['id','name','type','position','isburning']
    with open(filepath, mode='w', newline='', encoding='gb2312') as file:
        writer = csv.writer(file)  # 创建一个 writer 对象
        writer.writerow([size,])
        writer.writerow(header)
        writer.writerows(res)  # 写入多行

#查重
def is_existed(beg_id,end_id,testlist):
    for sub_list in testlist:
        if  (sub_list[0] == beg_id and sub_list[1] == end_id) or (sub_list[1] == beg_id and sub_list[0] == end_id):
            return True
    return False
#生成地图边
def GenerateEdges(Maxsize,filepath):
    type_choices = ["stair","corridor","road"]
    id_choices = [x for x in range(1,Maxsize+1)]
    resault = []
    header = ['begin_id','end_id','type','congestion','distance','riskLevel']
    minEdge = Maxsize   #最小边数
    maxEdge = Maxsize*(Maxsize-1)/2 #最大边数
    EdgeNum = random.randint(minEdge,maxEdge)   #选择边数
    for i in range(EdgeNum):
        beg_id = random.choice(id_choices)
        end_id = random.choice(id_choices)
        #查重+避免自环
        while beg_id == end_id or is_existed(beg_id,end_id,resault):
            beg_id = random.choice(id_choices)
            end_id = random.choice(id_choices)
        type = random.choice(type_choices)
        congestion = round(random.uniform(0.0,1.0),1)
        distance = random.randint(20,100)+round(random.random(),2)
        riskleval = random.randint(0,10)
        edge = [beg_id,end_id,type,congestion,distance,riskleval]
        resault.append(edge)
    with open(filepath, mode='a', newline='', encoding='gb2312') as file:  
        writer = csv.writer(file)  # 创建一个 writer 对象
        writer.writerow([EdgeNum,])
        writer.writerow(header)
        writer.writerows(resault)  # 写入多行


if __name__ == "__main__":
    print("请输入地图数量：")
    num = eval(input())
    print("请输入地图点个数：")
    size = eval(input())
    pos = "./Maps"
    os.makedirs(pos, exist_ok=True)  # 创建 Maps 目录
    for i in range(num):
        filename = '/map' + str(i) + '.csv'
        filepath = pos + filename  # 构造完整文件路径
        GeneratePointMap(size, filepath)
        GenerateEdges(size, filepath)
