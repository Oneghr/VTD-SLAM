#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<opencv2/core/core.hpp>

#include <opencv2/highgui/highgui.hpp>

using namespace std;

void LoadDepthImage(string sFile,vector<vector<string>> & file_n);
void LoopDetectAndInpainting(cv::Mat & vMat);
int main(int arg,char *argv[])
{
    if(arg==1) 
    {
        cout<<"<exe>./executable  <path to your dataset> "<<endl;
        return 0;
    }
    else
    {
        string setFile=string(argv[1])+"/associations.txt";
        vector<vector<string>> filename;
        LoadDepthImage(setFile,filename);
        for(int i=0;i<filename[0].size();i++)
        {
            //读取对应数据集的mask
            cv::Mat imMask=cv::imread("../w_halfsphere/"+filename[0][i]+".png",CV_LOAD_IMAGE_UNCHANGED);
            //读取深度图
            cv::Mat imDEP=cv::imread(string(argv[1])+"/"+filename[1][i],CV_LOAD_IMAGE_UNCHANGED);
            auto l=imDEP.type();

             cv::Mat bgr[3];
             cv::split(imMask,bgr);
             cv::Mat sig=bgr[0]/255;

            imDEP.convertTo(imDEP,CV_32F,1.0f/5000);
            sig.convertTo(sig,CV_32F);
            cv::Mat img=sig.mul(imDEP);
;
            cv::Mat myone=cv::Mat::ones(sig.size[0],sig.size[1],sig.type());
            cv::Mat J=myone-sig;

            //索引循环，剔除异常值
            LoopDetectAndInpainting(img);

            cv::Mat imD=img.mul(sig)+imDEP.mul(J);
            imD.convertTo(imD,l,5000.0f);
            cv::imwrite("../half/"+filename[2][i],imD);
            
         }

        
    }
    return 0;

}
void LoadDepthImage(string sFile,vector<vector<string>> & file_n)
{
    ifstream fin(sFile.c_str());
    string allLine;
    getline(fin,allLine);        
    vector<string> rgb_;
    vector<string> dep_;
    vector<string> dep_1;
    while(!allLine.empty())
    {

        string a1,a2,a3,a4;
        stringstream ss;
        ss<<allLine;
        ss>>a1;
        ss>>a2;
        ss>>a3;
        ss>>a4;
        // cout<<"a4: "<<a4<<endl;
        rgb_.push_back(a1);
        dep_.push_back(a4);
        dep_1.push_back(a3+".png");
        getline(fin,allLine);
    }
    file_n.push_back(rgb_);
    file_n.push_back(dep_);
    file_n.push_back(dep_1);

}
void LoopDetectAndInpainting(cv::Mat & vMat)
{
    
             for(int i=1;i<vMat.size[1];i++)
             {
                for(int j=1;j<vMat.size[0];j++)
                {
                    float d=vMat.at<float>(j,i);
                    if(d==0)
                    vMat.at<float>(j,i)=vMat.at<float>(j,i-1);
                }

             }

            for(int i=1;i<vMat.size[1];i++)
             {
                for(int j=1;j<vMat.size[0];j++)
                {
                    float d=vMat.at<float>(vMat.size[0]-j,vMat.size[1]-i);
                    if(d==0)
                    vMat.at<float>(vMat.size[0]-j,vMat.size[1]-i)=vMat.at<float>(vMat.size[0]-j,vMat.size[1]-i-1);
                }

             }

            for(int i=1;i<vMat.size[1];i++)
             {
                for(int j=1;j<vMat.size[0];j++)
                {
                    float d=vMat.at<float>(j,i);
                    if(d==0)
                    vMat.at<float>(j,i)=vMat.at<float>(j-1,i);
                }

             }

            for(int i=1;i<vMat.size[1];i++)
             {
                for(int j=1;j<vMat.size[0];j++)
                {
                    float d=vMat.at<float>(vMat.size[0]-j,vMat.size[1]-i);
                    if(d==0)
                    vMat.at<float>(vMat.size[0]-j,vMat.size[1]-i)=vMat.at<float>(vMat.size[0]-j-1,vMat.size[1]-i);
                }

             }


}