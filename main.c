
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <dirent.h>
#include <strings.h>
//shuoyixia zhege
#define LOWER_PAGE 				1
#define SLC_PAGE                0
#define UPPER_PAGE 				3
#define NO_UPPER_PAGE			0
#define GET_PAGE_TYPE(page)			((((page) & 1) || ((page) < 64)) ? LOWER_PAGE : UPPER_PAGE)

#define GET_UPPER_PAGE(page)	((((page) > 960) || ((page) < 32)) ? NO_UPPER_PAGE\
								: (((page) < 63) ? ((page) * 2) : ((page) + 63)))

#define GET_LOWER_PAGE(page)	(((page) < 126) ?  ((page) / 2) : ((page) - 63))

#define BLOCK_COUNT 1920
//#define BLOCK_COUNT 480
#define PAGE_COUNT 1024

typedef struct
{
    float sum;
    short max;
    int count;
}STATISTICS_NODE;

STATISTICS_NODE Word_Line_Node[2][34] = {0};// 存放每个在wordline，筛选出的不同类型所有page的信息。

unsigned short Page_max_value[PAGE_COUNT] = {0};
unsigned int Page_sum_value[PAGE_COUNT] = {0};
float Page_average_value[PAGE_COUNT] = {0};

unsigned int UniqueId = 0,BlockNum = 0,Endurance = 0;

void do_init()// 初始化 节点里的信息
{
    int i = 0,j = 0;
    for(i = 0; i < 2; i++)
    {
        for(j = 0;j < 34; j++)
        {
            Word_Line_Node[i][j].sum = 0;
            Word_Line_Node[i][j].max = 0;
            Word_Line_Node[i][j].count = 0;
        }
    }

    for(i = 0;i < PAGE_COUNT; i++)
    {
        Page_max_value[i] = 0;
    }
    for(i = 0;i < PAGE_COUNT; i++)
    {
        Page_sum_value[i] = 0;
    }
    for(i = 0;i < PAGE_COUNT; i++)
    {
        Page_average_value[i] = 0;
    }
}

unsigned int GetPageType(unsigned int p)
{//获取page的类型
    if(GET_PAGE_TYPE(p) == UPPER_PAGE)
        return UPPER_PAGE;
    if(GET_UPPER_PAGE(p) == NO_UPPER_PAGE)
        return SLC_PAGE;
    else
        return LOWER_PAGE;
}

void GetWordLineNumber(unsigned int p,unsigned int *number,unsigned int *type)
{//根据page，获取page所属的wordline，以及page的类型，并存储。
    *type = GetPageType(p);
    if(*type == UPPER_PAGE)
        p = GET_LOWER_PAGE(p);

    if(p < 64)
        *number = p / 16;
    else if(p < 960)
        *number = (p / 32) + 2;
    else
        *number = (p - 960) / 32 + 32;
}

void DistributionRowColumn(unsigned int number,unsigned int type,unsigned int *row,unsigned int *column)
{// 对应到相应的二维数组上，即数据的一个元素代表一个wordline里，某种类型的所有page的总的信息，如：sum，max，count。
    if(type == UPPER_PAGE)
    {
        *row = 1;
        *column = number;
    }
    else if(type == SLC_PAGE)
    {
        *row = 0;
        *column = number;
    }
    else
    {
        *row = 0;
        *column = number;
    }
}

void Select_Data(unsigned int row,unsigned int column,short value)
{// 获取每个节点内部的信息
    Word_Line_Node[row][column].count ++;
    Word_Line_Node[row][column].sum += value;
    if(Word_Line_Node[row][column].max < value)
        Word_Line_Node[row][column].max = value;
}

unsigned int c2h(unsigned char c)
{//字符装换为数字
    if((c >= '0')
            && (c <= '9'))
        return c - '0';
    else if((c >= 'A')
            && (c <= 'F'))
        return c - 'A' + 10;
    else if((c >= 'a')
            && (c <= 'f'))
        return c - 'a' + 10;
    else
        return 0x100;
}

/*Change one string to int*/
unsigned int do_int(char *fp,int length)
{ //字符串转换为16进制数
    int i,j = 0,sum = 0;//,temp_length = 0;
    for(i = 0; i < length; i++)
    {
        j = c2h(fp[i]);
        if(j > 15)
        {
            break;
        }
        else
        {
            sum = sum * 16 + j;
        }
    }
    return sum;
}

void different_wordline(struct dirent * fileinfo)
{// 拿出目录中的rdber文件信息流
    FILE *fp = NULL;
    int file_length = 0;
    short first_value = 0,second_value = 0;
    int codeword = 0,page = 0;
    char result_name[32] = {0};
    unsigned int word_line_number = 0,page_type = 0;
    unsigned int row = 0,column = 0;

    if((fp = fopen(fileinfo->d_name,"rb+")) == NULL)
    {
        perror("fail to fopen");
        return;
    }

    fseek(fp,0,SEEK_END);//fp指向文件流末尾
    file_length = ftell(fp);// 从文件开头到fp指向的位置文件字节数
    fseek(fp,0,SEEK_SET);//fp再次指向文件开头

    while(file_length > 0)
    {
        fread(&first_value,2,1,fp);//读取一个单元，每个单元为两个字节
        fread(&second_value,2,1,fp);

        GetWordLineNumber(page, &word_line_number, &page_type);//获取wordline号以及page类型
        DistributionRowColumn(word_line_number, page_type, &row, &column);//得到所属的行号列号
        Select_Data(row, column , (first_value + second_value)); //存储相应数组元素信息


        codeword ++;
        if(codeword >= 4)
        {// 决定页号的下移
            page += 1;
            codeword = 0;
        }

        file_length = file_length - 4; // 决定何时读完跳出文件
    }
    fclose(fp);
}

void write_into_file(char* dir_path,int endurance_number,char *dest_dir_name,char* big_dir)
{
    char result_name[256] = {0};
    char dest_path[256] = {0};
    char command[512] = {0};
    FILE *result = NULL;
    DIR *dir = NULL;
    struct dirent * dir_file = NULL;

    int i ,j;

    chdir(dir_path);

    sprintf(result_name,"%s/different_word_line_result_%04d.txt",dir_path,endurance_number);
    if((result = fopen(result_name,"w+")) == NULL)
    {
        perror("fail to fopen");
        return ;
    }

    fprintf(result,"wordline_number       max        average\n");
    fprintf(result,"slc  :0             %4d        %6.2f\n",Word_Line_Node[0][0].max,Word_Line_Node[0][0].sum/Word_Line_Node[0][0].count);
    fprintf(result,"slc  :1             %4d        %6.2f\n",Word_Line_Node[0][1].max,Word_Line_Node[0][1].sum/Word_Line_Node[0][1].count);
    fprintf(result,"slc  :32            %4d        %6.2f\n",Word_Line_Node[0][32].max,Word_Line_Node[0][32].sum / Word_Line_Node[0][32].count);
    fprintf(result,"slc  :33            %4d        %6.2f\n",Word_Line_Node[0][33].max,Word_Line_Node[0][33].sum / Word_Line_Node[0][33].count);

    fprintf(result,"wl_num    lower_max    lower_average   |    upper_max   upper_average\n");

    for(j = 2;j < 32;j++)
    {
        fprintf(result,"%2d          %4d        %6.2f         |       %4d        %6.2f\n",j,Word_Line_Node[0][j].max,Word_Line_Node[0][j].sum / Word_Line_Node[0][j].count,
                                                                                                    Word_Line_Node[1][j].max,Word_Line_Node[1][j].sum / Word_Line_Node[1][j].count);
    }

    fclose(result);
    sprintf(dest_path,"%s/different_word_line",big_dir);
    if((dir = opendir(dest_path)) == NULL)
    {
        perror("fail to opendir!");
        return ;
    }
    while((dir_file = readdir(dir)) != NULL)
    {
        if((dir_file->d_name[0] == dest_dir_name[0]) && (dir_file->d_name[1] == dest_dir_name[1]))
            sprintf(dest_path,"%s/%s",dest_path,dir_file->d_name);
    }
    sprintf(command,"copy %s %s",result_name,dest_path);
    printf("%s\n",command);
    system(command);
    closedir(dir);
}

void statistics_a_directory(char *path,char *dest_dir_name,char*big_dir)
{//1.rdber文件所在的目录 ，如：\fifth_test\normal_tempurature_7\2_pattern\5055_4_0_0_0\all_nand
    DIR *dir = NULL;
    struct dirent * file_info = NULL;
    int count_number = 0;
    int endurance_number = 0;
    while(endurance_number <= 4)// 根据Endurance先将block分类，然后
    {
        if((dir = opendir(path)) == NULL)// 打开目录得到一个目录流
        {
            perror("fail to opendir");
            return -1;
        }
        do_init();
        count_number = 0;
        chdir(path);
        while((file_info = readdir(dir)) != NULL)// 读取目录流，得到文件的信息，然后下移
        {
            if(file_info->d_name[0] == '.')
                continue;
            if(strncmp(file_info->d_name,"rdber",5) != 0)
                continue;

            UniqueId = do_int(file_info->d_name + 6,8);

            BlockNum = do_int(file_info->d_name + 15,4);

            Endurance = do_int(file_info->d_name + 20,4);
            if(Endurance != endurance_number)
                continue;//根据Endurance的次序处理block

            different_wordline(file_info);
            count_number ++;
        }
        printf("count_number:%d\n",count_number);
        write_into_file(path,endurance_number,dest_dir_name,big_dir);
        // 将Endurance相同的block，分析得到数据存储在同一个文件里
        printf("%d finished!\n",endurance_number);
        endurance_number ++;// 分析另外的Endurance下的block
        closedir(dir);  // 关闭目录流
    }
}
#if 0
void copy_result_to_dest_dir(char *path)
{
    DIR *dir_bake = NULL,*dir_offset = NULL;
    struct dirent *info_bake = NULL, *info_offset = NULL;
    char bake_path[128] = {0},offset_path[128] = {0},search_dir_name[6] = {0};

    if((dir_bake = opendir(path)) == NULL)
    {
        perror("fail to opendir");
        return -1;
    }
    while((info_bake = readdir(dir_bake)) != NULL)
    {
        strcpy(bake_path,path);
        if(info_bake->d_name[0] == '.')
            continue;
        if(strncmp(info_bake->d_name,"5055",4) != 0)
            continue;
        strncpy(search_dir_name,info_bake->d_name + 11,2);
        if(info_bake->d_type != DT_DIR)
            continue;
        strcat(bake_path,"/");
        strcat(bake_path,info_bake->d_name);
        //printf("%s\n",bake_path);
        if((dir_offset = opendir(bake_path)) == NULL)
        {
            perror("fail to opendir");
            return -1;
        }
        while((info_offset = readdir(dir_offset)) != NULL)
        {
            strcpy(offset_path,bake_path);
            if(info_offset->d_name[0] == '.')
                continue;
            if(info_offset->d_type != DT_DIR)
                continue;
#if 1
            if(strncmp(info_offset->d_name,"all_nand",8) != 0)
                continue;
#endif
            strcat(offset_path,"/");
            strcat(offset_path,info_offset->d_name);

            //printf("%s\n",offset_path);

            memset(offset_path,0,sizeof(offset_path));
        }
        closedir(dir_offset);

        memset(bake_path,0,sizeof(bake_path));
        memset(search_dir_name,0,sizeof(search_dir_name));

    }

}
#endif
int main(int argc,char *argv[])
{
    DIR *dir_bake = NULL,*dir_offset = NULL;
    struct dirent *info_bake = NULL, *info_offset = NULL,search_dir_name[6] = {0};
    char bake_path[256] = {0},offset_path[256] = {0};
    if(argc < 2)
    {
        printf("Usage:%s dir path.\n",argv[0]);
        return -1;
    }
    if((dir_bake = opendir(argv[1])) == NULL)
    {
        perror("fail to opendir");
        return -1;
    }

    while((info_bake = readdir(dir_bake)) != NULL)
    {
        strcpy(bake_path,argv[1]);
        if(info_bake->d_name[0] == '.')
            continue;
        if(strncmp(info_bake->d_name,"5055",4) != 0)
            continue;
        strncpy(search_dir_name,info_bake->d_name + 11,2);
        //printf("offset:%s..............\n",search_dir_name);
        if(info_bake->d_type != DT_DIR)
            continue;
        strcat(bake_path,"/");
        strcat(bake_path,info_bake->d_name);
        //printf("%s\n",bake_path);
        if((dir_offset = opendir(bake_path)) == NULL)
        {
            perror("fail to opendir");
            return -1;
        }
        while((info_offset = readdir(dir_offset)) != NULL)
        {
            strcpy(offset_path,bake_path);
            if(info_offset->d_name[0] == '.')
                continue;
            if(info_offset->d_type != DT_DIR)
                continue;
#if 1
            if(strncmp(info_offset->d_name,"all_nand",8) != 0)
                continue;
#endif
            strcat(offset_path,"/");
            strcat(offset_path,info_offset->d_name);
            //printf("%s\n",offset_path);
            statistics_a_directory(offset_path,search_dir_name,argv[1]);

            memset(offset_path,0,sizeof(offset_path));
        }
        closedir(dir_offset);

        memset(bake_path,0,sizeof(bake_path));
        memset(search_dir_name,0,sizeof(search_dir_name));
    }
    closedir(dir_bake);
    //copy_result_to_dest_dir(argv[1]);


    return 0;
}
