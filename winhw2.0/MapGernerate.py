import random
import csv
import os
#边查重
def is_existed(beg_id,end_id,testlist):
    for sub_list in testlist:
        if  (sub_list[0] == beg_id and sub_list[1] == end_id) or (sub_list[1] == beg_id and sub_list[0] == end_id):
            return True
    return False

#生成地图，size为点的数量
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
        e_xposition = round(random.uniform(0.00,20.00),2)
        e_yposition = round(random.uniform(0.00,20.00),2)
        e_isburning = random.choice([0,0,0,1]) if e_type != 'EXIT' else 0
        element.append(e_name)
        element.append(e_type)
        element.append(e_xposition)
        element.append(e_yposition)
        element.append(e_isburning)
        res.append(element)
    header = ['id','name','type','xposition','ypositon','isburning']
    with open(filepath, mode='w', newline='', encoding='gb2312') as file:
        writer = csv.writer(file)  # 创建一个 writer 对象
        writer.writerow([size,])
        writer.writerow(header)
        writer.writerows(res)  # 写入多行
#生成地图边
    type_choices = ["stair","corridor","road"]
    id_choices = [x for x in range(1,size+1)]
    resault = []
    header = ['begin_id','end_id','type','congestion','distance','riskLevel','intermediate_point_x','intermediate_point_y']
    minEdge = size   #最小边数
    maxEdge = size*(size-1)/2 #最大边数
    EdgeNum = random.randint(minEdge,maxEdge)   #选择边数
    for i in range(EdgeNum):
        beg_id = random.choice(id_choices)
        end_id = random.choice(id_choices)
        #查重+避免自环
        while beg_id == end_id or is_existed(beg_id,end_id,resault):
            beg_id = random.choice(id_choices)
            end_id = random.choice(id_choices)
        #获取该边起终点的坐标
        beg_x = res[beg_id-1][3]
        beg_y = res[beg_id-1][4]
        end_x = res[end_id-1][3]
        end_y = res[end_id-1][4]
        #从两个对角点选一个当中转点并计算边的总长
        if random.choice([True,False]):
            intermediate_point_x = beg_x
            intermediate_point_y = end_y
        else:
            intermediate_point_x = end_x
            intermediate_point_y = beg_y
        distance = round((abs(beg_x-end_x)+abs(end_x-end_y)),2)
        type = random.choice(type_choices)
        congestion = round(random.uniform(0.0,1.0),1)
        riskleval = random.randint(0,10)
        edge = [beg_id,end_id,type,congestion,distance,riskleval,intermediate_point_x,intermediate_point_y]
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
        #GenerateEdges(size, filepath)
