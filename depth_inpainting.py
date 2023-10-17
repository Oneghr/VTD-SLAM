import cv2
import os
import numpy as np
from PIL import Image

from pathlib import Path
ss="static"
dataset_path="/home/qrb/datasets/Data/rgbd_dataset_freiburg3_walking_"+ss

orange_rgb=[8/90,46/90,84/90]


rgb_list=[]
dep_list=[]
b=True
f=open(dataset_path+"/associations.txt",'r')
while(b):
    r=f.readline()
    if not r:
        b=False
    else:
        content=r.split(" ")
        rgb_list.append("./w_"+ss+"/"+content[0]+".png")
        dep_list.append(dataset_path+"/depth/"+content[2]+".png")

        #print(content)
print("共有"+str(len(rgb_list))+"张图片待处理...")
for i in range(len(rgb_list)):
    imMask=cv2.imread(rgb_list[i],0)
    IM=np.zeros((imMask.shape[0],imMask.shape[1]))
    IMi=np.ones((imMask.shape[0],imMask.shape[1]))
    IM[imMask[:,:]!=0]=1
    imD=cv2.imread(dep_list[i])
    #print(type(imD))
    #J=imD*IM
    J=np.ones((imD.shape[0],imD.shape[1],3))
    J[:,:,0]=imD[:,:,0]*IM
    J[:,:,1]=imD[:,:,1]*IM
    J[:,:,2]=imD[:,:,2]*IM
    #cv2.imwrite("./f/"+str(i+1)+".png",J)
    # 
    for x in range(imD.shape[1]):
        for y in range(imD.shape[0]):
            d=J[y,x][0]
            if(d==0):
                J[y,x]=[J[y,x-1][0],J[y,x-1][0],J[y,x-1][0]]

    for x in range(imD.shape[1]):
        for y in range(imD.shape[0]):
            d=J[y,x][0]
            if(d==0):
                J[y,x]=[J[y-1,x][0],J[y-1,x][0],J[y-1,x][0]]

    for x in range(1,imD.shape[1]):
        for y in range(1,imD.shape[0]):
            d=J[imD.shape[0]-y,imD.shape[1]-x][0]
            if(d==0 and imD.shape[1]-x-1>=0):
                J[imD.shape[0]-y,imD.shape[1]-x]=[J[imD.shape[0]-y,imD.shape[1]-x-1][0],J[imD.shape[0]-y,imD.shape[1]-x-1][0],J[imD.shape[0]-y,imD.shape[1]-x-1][0]]


    for x in range(1,imD.shape[1]):
        for y in range(1,imD.shape[0]):
            d=J[imD.shape[0]-y,imD.shape[1]-x][0]
            if(d==0 and imD.shape[0]-y-1>=0):
                J[imD.shape[0]-y,imD.shape[1]-x]=[J[imD.shape[0]-y-1,imD.shape[1]-x][0],J[imD.shape[0]-y-1,imD.shape[1]-x][0],J[imD.shape[0]-y-1,imD.shape[1]-x][0]]

    dep_name=dep_list[i].split("/")
    #print(dep_name[7])
    path_=ss+"_depth_inpainted/"
    path_1=ss+"_depth_inpainted_rgb/"

    output_dir = Path(path_)
    output_dir.mkdir(exist_ok=True)

    # output_dir1 = Path(path_1)
    # output_dir1.mkdir(exist_ok=True)

    J[:,:,0]=J[:,:,0]*IM+imD[:,:,0]*(IMi-IM)
    J[:,:,1]=J[:,:,1]*IM+imD[:,:,0]*(IMi-IM)
    J[:,:,2]=J[:,:,2]*IM+imD[:,:,0]*(IMi-IM)

    cv2.imwrite("./"+ss+"_depth_inpainted/"+dep_name[7],J)

    # for x in range(imD.shape[1]):
    #     for y in range(imD.shape[0]):
    #             J[y,x]=[int(J[y,x][0]*orange_rgb[2]),int(J[y,x][1]*orange_rgb[1]),int(J[y,x][2]*orange_rgb[0])]
    # cv2.imwrite("./"+ss+"_depth_inpainted_rgb/"+dep_name[7],J)


    print("第"+str(i+1)+"图片处理完毕！")
