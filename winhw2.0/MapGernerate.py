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
    boarder = 20.00 #地图边界
    scale = round(size*3/4)
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
        #强制限制3/4的点落在9/16的面积里，使得点分布更集中
        if i<scale:
            e_xposition = round(random.uniform(boarder/8,boarder*7/8),2)
            e_yposition = round(random.uniform(boarder/8,boarder*7/8),2)
        else:
            e_xposition = round(random.uniform(0.00,boarder),2)
            e_yposition = round(random.uniform(0.00, boarder), 2)
        e_isburning = random.choice([0,0,0,1]) if e_type != 'EXIT' else 0
        element.append(e_name)
        element.append(e_type)
        element.append(e_xposition)
        element.append(e_yposition)
        element.append(e_isburning)
        res.append(element)

#生成地图边
    type_choices = ["stair","corridor","road"]
    id_choices = [x for x in range(1,size+1)]
    resault = []

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
        #选取大边
        #获取该边起终点的坐标
        beg_x = res[beg_id-1][3]
        beg_y = res[beg_id-1][4]
        end_x = res[end_id-1][3]
        end_y = res[end_id-1][4]
        new_id = size + i + 1
        #从两个对角点选一个当中转点并计算边的总长
        if random.choice([True,False]):
            intermediate_point_x = beg_x
            intermediate_point_y = end_y
            distance1 = round(abs(beg_y - intermediate_point_y), 2)
            distance2 = round(abs(end_x - intermediate_point_x), 2)
        else:
            intermediate_point_x = end_x
            intermediate_point_y = beg_y
            distance1 = round(abs(beg_x - intermediate_point_x), 2)
            distance2 = round(abs(end_y - intermediate_point_y), 2)

        #中转点进入点集
        new_point = [new_id,'中转点','intermediate',intermediate_point_x,intermediate_point_y,0]
        res.append(new_point)
        #构造两边
        type = random.choice(type_choices)
        congestion = round(random.uniform(0.0,1.0),1)
        riskleval = random.randint(0,10)
        edge1 = [beg_id,new_id,type,congestion,distance1,riskleval]
        edge2 = [new_id,end_id,type,congestion,distance2,riskleval]
        resault.append(edge1)
        resault.append(edge2)
    #写入点
    header = ['id', 'name', 'type', 'xposition', 'ypositon', 'isburning']
    with open(filepath, mode='w', newline='', encoding='gb2312') as file:
            writer = csv.writer(file)  # 创建一个 writer 对象
            writer.writerow([size+EdgeNum, ])
            writer.writerow(header)
            writer.writerows(res)  # 写入多行
    #写入边
    header = ['begin_id', 'end_id', 'type', 'congestion', 'distance', 'riskLevel']
    with open(filepath, mode='a', newline='', encoding='gb2312') as file:  
        writer = csv.writer(file)  # 创建一个 writer 对象
        writer.writerow([EdgeNum*2,])
        writer.writerow(header)
        writer.writerows(resault)  # 写入多行


if __name__ == "__main__":
    print("请输入地图数量：")
    num = eval(input())
    print("请输入地图有效点点个数：")
    size = eval(input())
    pos = "./Maps"
    os.makedirs(pos, exist_ok=True)  # 创建 Maps 目录
    for i in range(num):
        filename = '/map' + str(i) + '.csv'
        filepath = pos + filename  # 构造完整文件路径
        GeneratePointMap(size, filepath)