#include <iostream>
#include <stdio.h>
#include <time.h>
#include <math.h>

void k_means(int n,int k, int n_a, double **x, int *y);
double Dofclu(double **x, int i, double **cluster, int j, int n_a);
int find_k(double **x, int n, double **k, int k_n, double **D, int n_a);
char clu[5] = {'A', 'B', 'C', 'D', 'E'};

int main(){
    srand(time(NULL));
    int n,k,n_a;
    int i,j,a;
    std::cout << "输入样本数、簇个数、属性个数：\n簇个数不要超过5个！\n";
    std::cin >> n >> k >> n_a;
    double **x = new double*[n];
    for(i=0;i<n;i++){
        x[i] = new double[n_a];
    }
    int *y = new int[n]();

    std::cout << "输入每个样本的属性，每行一个样本\n共有" << n 
                << "行,每行" << n_a << "个属性：\n";

    for(i=0;i<n;i++){
        for(j=0;j<n_a;j++){
            std::cin >> x[i][j];
            //std::cout << "x[" << i << "]" << "的属性[" << j << "]为" << x[i][j] << std::endl;
        }
    }

    std::cout << "//---------------------------\n\n";
    k_means(n, k, n_a, x, y);
    std::cout << "//~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";

    for(i=0;i<k;i++){
        std::cout << "划分为" << clu[i] << "簇的样本为：\n";
        for(j=0;j<n;j++){
            if(y[j] == i){
                std::cout << "x[" << j << "], 属性为:";
                for(a=0;a<n_a;a++){
                    std::cout << x[j][a] << ", ";
                }
                std::cout << std::endl;
            }
        }
    }

    delete[] x;
    delete[] y;
    return 0;
}

void k_means(int n,int k, int n_a, double **x, int *y){
    int i,j,r_, T=0, a;
    //簇[k个簇][n_a个属性]
    double **cluster = new double*[k]();
    for(i=0;i<k;i++){
        cluster[i] = new double[n_a]();
    }
    //距离[该样本][对每个簇]
    double **D = new double*[n]();
    for(i=0;i<n;i++){
        D[i] = new double[k]();
    }
    //上一代迭代的簇属性
    double **last_clt = new double*[k]();
    for(i=0;i<k;i++){
        last_clt[i] = new double[n_a]();
    }
    //随机簇中心
    int *cr = new int[k]();
    r_ = rand()%n;
    cr[0] = r_;
    for(i=0;i<n_a;i++){
        cluster[0][i] = x[r_][i];
    }
    for(i=1;i<k;i++){
        j=0;
        while(j<i){//找到不重复的样本下标
            r_ = find_k(x, n, cluster, i+1, D, n_a);
            if(r_ == cr[j]) continue;
            j++;
        }
        cr[i] = r_;
        
        for(j=0;j<n_a;j++){
            cluster[i][j] = x[r_][j];//复制属性
        }
    }
    for(i=0;i<k;i++){
        std::cout << "x[" << cr[i] << "]被选为初始" << clu[i] << "簇中心~\n";
    }

    while(T!=15){//算法迭代次数
        //遍历样本，根据最短距离划分簇
        double MinD=0;
        std::cout << "第" << T+1 << "次迭代结果\n";
        for(i=0;i<n;i++){
            MinD = 0;
            for(j=0;j<k;j++){//每个样本关于每个簇的距离
                D[i][j] = Dofclu(x, i, cluster, j, n_a);
            }
            MinD = D[i][0];
            y[i] = 0;
            for(j=0;j<k;j++){
                if(D[i][j] < MinD){
                    MinD = D[i][j];
                    y[i] = j;//样本相应下标的划分变为相应簇下标
                }
            }
            std::cout << "样本x[" << i << "]被划分为" << clu[y[i]] << "簇\n";
        }

        //将各个簇中样本的属性平均值作为新的簇中心
        std::cout << "更新簇中心：\n    此代簇中心：\n";
        for(i=0;i<k;i++){
            std::cout << clu[i] << "簇属性值：";
            for(j=0;j<n_a;j++){
                std::cout << cluster[i][j] << " ";
            }
            std::cout << std::endl;
        }
        for(i=0;i<k;i++){
            r_ = 0;
            for(j=0;j<n_a;j++){//当前簇的属性归零
                last_clt[i][j] =cluster[i][j];
                cluster[i][j] = 0;
            }
            for(j=0;j<n;j++){
                if(i==y[j]){//如果样本下标的划分是当前簇
                    for(a=0;a<n_a;a++){
                        cluster[i][a] += x[j][a];
                    }
                    r_++;
                }
            }
            for(j=0;j<n_a;j++){
                cluster[i][j] /= r_;
            }
        }
        std::cout << "    次代簇中心：\n";
        for(i=0;i<k;i++){
            std::cout << clu[i] << "簇属性值：";
            for(j=0;j<n_a;j++){
                std::cout << cluster[i][j] << " ";
            }
            std::cout << std::endl;
        }

        //将新簇中心与上一代簇中心对比，无变化则算法结束
        r_ = 1;//标记
        for(i=0;i<k;i++){
            for(j=0;j<n_a;j++){
                if(last_clt[i][j] != cluster[i][j]){
                    r_ = 0;
                    break;
                }
            }
            if(r_ == 0) break;
        }
        if(r_ == 1) break;
        T++;
    }

    std::cout << "迭代完毕，迭代次数：" << T+1 << std::endl;

    for(i=0;i<k;i++){
        std::cout << clu[i] << "簇中心的属性值为";
        for(j=0;j<n_a;j++){
            std::cout << cluster[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    delete cr;
    delete[] last_clt;
    delete[] D;
    delete[] cluster;
}
//计算距离（样本数组，样本下标， 簇数组， 簇下标， 属性个数）
double Dofclu(double **x, int i, double **cluster, int j, int n_a){
    int a;
    double D=0;
    for(a=0;a<n_a;a++){
        D += (x[i][a] - cluster[j][a]) * (x[i][a] - cluster[j][a]);
    }
    return sqrt(D);
}
//寻找新簇中心，返回合适的样本下标（样本数组， 样本数， 簇数组， 簇数， 距离数组， 属性个数）
int find_k(double **x, int n, double **k, int k_n, double **D, int n_a){
    int i,j;
    double *Min = new double[n]();
    double *P = new double[n]();
    double SumD=0;

    for(i=0;i<n;i++){
        for(j=0;j<k_n;j++){
            D[i][j] = Dofclu(x, i, k, j, n_a);
        }
    }
    for(i=0;i<n;i++){
        Min[i] = D[i][0];
        for(j=0;j<k_n;j++){
            if(D[i][j] < Min[i])
                Min[i] = D[i][j];
        }
    }
    for(i=0;i<n;i++){
        SumD += Min[i] * Min[i];
    }
    for(i=0;i<n;i++){
        P[i] = Min[i] * Min[i] / SumD;
    }

    j = rand() / (RAND_MAX + 1.0);//此处j为随机下标，不做循环用
    SumD = 0;
    for(i=0;i<n;i++){
        SumD += P[i];
        if(SumD > j)
            break;
    }

    delete Min;
    delete P;
    return i;
}