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
    std::cout << "�������������ظ��������Ը�����\n�ظ�����Ҫ����5����\n";
    std::cin >> n >> k >> n_a;
    double **x = new double*[n];
    for(i=0;i<n;i++){
        x[i] = new double[n_a];
    }
    int *y = new int[n]();

    std::cout << "����ÿ�����������ԣ�ÿ��һ������\n����" << n 
                << "��,ÿ��" << n_a << "�����ԣ�\n";

    for(i=0;i<n;i++){
        for(j=0;j<n_a;j++){
            std::cin >> x[i][j];
            //std::cout << "x[" << i << "]" << "������[" << j << "]Ϊ" << x[i][j] << std::endl;
        }
    }

    std::cout << "//---------------------------\n\n";
    k_means(n, k, n_a, x, y);
    std::cout << "//~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";

    for(i=0;i<k;i++){
        std::cout << "����Ϊ" << clu[i] << "�ص�����Ϊ��\n";
        for(j=0;j<n;j++){
            if(y[j] == i){
                std::cout << "x[" << j << "], ����Ϊ:";
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
    //��[k����][n_a������]
    double **cluster = new double*[k]();
    for(i=0;i<k;i++){
        cluster[i] = new double[n_a]();
    }
    //����[������][��ÿ����]
    double **D = new double*[n]();
    for(i=0;i<n;i++){
        D[i] = new double[k]();
    }
    //��һ�������Ĵ�����
    double **last_clt = new double*[k]();
    for(i=0;i<k;i++){
        last_clt[i] = new double[n_a]();
    }
    //���������
    int *cr = new int[k]();
    r_ = rand()%n;
    cr[0] = r_;
    for(i=0;i<n_a;i++){
        cluster[0][i] = x[r_][i];
    }
    for(i=1;i<k;i++){
        j=0;
        while(j<i){//�ҵ����ظ��������±�
            r_ = find_k(x, n, cluster, i+1, D, n_a);
            if(r_ == cr[j]) continue;
            j++;
        }
        cr[i] = r_;
        
        for(j=0;j<n_a;j++){
            cluster[i][j] = x[r_][j];//��������
        }
    }
    for(i=0;i<k;i++){
        std::cout << "x[" << cr[i] << "]��ѡΪ��ʼ" << clu[i] << "������~\n";
    }

    while(T!=15){//�㷨��������
        //����������������̾��뻮�ִ�
        double MinD=0;
        std::cout << "��" << T+1 << "�ε������\n";
        for(i=0;i<n;i++){
            MinD = 0;
            for(j=0;j<k;j++){//ÿ����������ÿ���صľ���
                D[i][j] = Dofclu(x, i, cluster, j, n_a);
            }
            MinD = D[i][0];
            y[i] = 0;
            for(j=0;j<k;j++){
                if(D[i][j] < MinD){
                    MinD = D[i][j];
                    y[i] = j;//������Ӧ�±�Ļ��ֱ�Ϊ��Ӧ���±�
                }
            }
            std::cout << "����x[" << i << "]������Ϊ" << clu[y[i]] << "��\n";
        }

        //��������������������ƽ��ֵ��Ϊ�µĴ�����
        std::cout << "���´����ģ�\n    �˴������ģ�\n";
        for(i=0;i<k;i++){
            std::cout << clu[i] << "������ֵ��";
            for(j=0;j<n_a;j++){
                std::cout << cluster[i][j] << " ";
            }
            std::cout << std::endl;
        }
        for(i=0;i<k;i++){
            r_ = 0;
            for(j=0;j<n_a;j++){//��ǰ�ص����Թ���
                last_clt[i][j] =cluster[i][j];
                cluster[i][j] = 0;
            }
            for(j=0;j<n;j++){
                if(i==y[j]){//��������±�Ļ����ǵ�ǰ��
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
        std::cout << "    �δ������ģ�\n";
        for(i=0;i<k;i++){
            std::cout << clu[i] << "������ֵ��";
            for(j=0;j<n_a;j++){
                std::cout << cluster[i][j] << " ";
            }
            std::cout << std::endl;
        }

        //���´���������һ�������ĶԱȣ��ޱ仯���㷨����
        r_ = 1;//���
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

    std::cout << "������ϣ�����������" << T+1 << std::endl;

    for(i=0;i<k;i++){
        std::cout << clu[i] << "�����ĵ�����ֵΪ";
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
//������루�������飬�����±꣬ �����飬 ���±꣬ ���Ը�����
double Dofclu(double **x, int i, double **cluster, int j, int n_a){
    int a;
    double D=0;
    for(a=0;a<n_a;a++){
        D += (x[i][a] - cluster[j][a]) * (x[i][a] - cluster[j][a]);
    }
    return sqrt(D);
}
//Ѱ���´����ģ����غ��ʵ������±꣨�������飬 �������� �����飬 ������ �������飬 ���Ը�����
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

    j = rand() / (RAND_MAX + 1.0);//�˴�jΪ����±꣬����ѭ����
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