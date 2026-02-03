import random
import csv
import os
def Generatesrcmap(size,filepath):
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
    filepath += '/srcmap.csv'
    header = ['id','name','type','position','isburning']
    with open(filepath, mode='w', newline='', encoding='gb2312') as file:
        writer = csv.writer(file)  # 创建一个 writer 对象
        writer.writerow(header)
        writer.writerows(res)  # 写入多行

def GenerateArrs(Maxsize,filepath):
    JamArr = []
    TypeArr = []
    SaveArr = []
    DisArr = []
    types = ["stair","corridor","road"]
    for i in range(Maxsize):
        for j in range(Maxsize):
            is_used = random.choice([False,True])
            if i > j :
                SaveArr.append(SaveArr[j*Maxsize+j+i])
                DisArr.append(DisArr[j*Maxsize+j+i])
                JamArr.append(JamArr[j*Maxsize+j+i])
                TypeArr.append(TypeArr[j*Maxsize+j+i])
                continue
            elif i == j or not is_used:
                SaveArr.append("-1 ")
                DisArr.append("-1 ")
                JamArr.append("-1.0 ")
                TypeArr.append("NONE ")
                continue
            SaveArr.append(str(random.randint(0,10))+' ')
            TypeArr.append(random.choice(types)+' ')
            DisArr.append(str(random.randint(20,100)+round(random.random(),2))+' ')
            JamArr.append(str(round(random.uniform(0.0,1.0),1))+' ')
        SaveArr.append('\n')
        DisArr.append('\n')
        JamArr.append('\n')
        TypeArr.append('\n')
    writetxt(filepath + "/TypeArr.txt",TypeArr)
    writetxt(filepath + "/SaveArr.txt",SaveArr)
    writetxt(filepath + "/JamArr.txt",JamArr)
    writetxt(filepath + "/DisArr.txt",DisArr)

def writetxt(filepath,list_in):
    with open(filepath,"w",encoding="gb2312") as file:
        file.writelines(list_in)

if __name__ == "__main__":
    print("请输入地图个数：")
    num = eval(input())
    print("请输入地图点个数：")
    size = eval(input())
    pos = "Maps"
    for i in range(num):
        Mappos = pos + '/map' + str(i)
        os.makedirs(Mappos,exist_ok=True)
        Generatesrcmap(size,Mappos)
        GenerateArrs(size,Mappos)