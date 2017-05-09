
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

STATISTICS_NODE Word_Line_Node[2][34] = {0};// å­˜æ”¾æ¯ä¸ªåœ¨wordlineï¼Œç­›é€‰å‡ºçš„ä¸åŒç±»åž‹æ‰€æœ‰pageçš„ä¿¡æ¯ã€‚

unsigned short Page_max_value[PAGE_COUNT] = {0};
unsigned int Page_sum_value[PAGE_COUNT] = {0};
float Page_average_value[PAGE_COUNT] = {0};

unsigned int UniqueId = 0,BlockNum = 0,Endurance = 0;

void do_init()// åˆå§‹åŒ– èŠ‚ç‚¹é‡Œçš„ä¿¡æ¯
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

<<<<<<< HEAD
unsigned int GetPageType(unsigned int p) //»ñÈ¡pageµÄÀàÐÍ
{
=======
unsigned int GetPageType(unsigned int p)
{//èŽ·å–pageçš„ç±»åž‹
>>>>>>> parent of bca47d5... å®žéªŒ
    if(GET_PAGE_TYPE(p) == UPPER_PAGE)
        return UPPER_PAGE;
    if(GET_UPPER_PAGE(p) == NO_UPPER_PAGE)
        return SLC_PAGE;
    else
        return LOWER_PAGE;
}

<<<<<<< HEAD
void GetWordLineNumber(unsigned int p,unsigned int *number,unsigned int *type) //¸ù¾Ýpage£¬»ñÈ¡pageËùÊôµÄwordline£¬ÒÔ¼°pageµÄÀàÐÍ£¬²¢´æ´¢¡£
{
=======
void GetWordLineNumber(unsigned int p,unsigned int *number,unsigned int *type)
{//æ ¹æ®pageï¼ŒèŽ·å–pageæ‰€å±žçš„wordlineï¼Œä»¥åŠpageçš„ç±»åž‹ï¼Œå¹¶å­˜å‚¨ã€‚
>>>>>>> parent of bca47d5... å®žéªŒ
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

<<<<<<< HEAD
void DistributionRowColumn(unsigned int number,unsigned int type,unsigned int *row ,unsigned int *column) //// ¶ÔÓ¦µ½ÏàÓ¦µÄ¶þÎ¬Êý×éÉÏ£¬¼´Êý¾ÝµÄÒ»¸öÔªËØ´ú±íÒ»¸öwordlineÀï£¬Ä³ÖÖÀàÐÍµÄËùÓÐpageµÄ×ÜµÄÐÅÏ¢£¬Èç£ºsum£¬max£¬count¡£
{
=======
void DistributionRowColumn(unsigned int number,unsigned int type,unsigned int *row,unsigned int *column)
{// å¯¹åº”åˆ°ç›¸åº”çš„äºŒç»´æ•°ç»„ä¸Šï¼Œå³æ•°æ®çš„ä¸€ä¸ªå…ƒç´ ä»£è¡¨ä¸€ä¸ªwordlineé‡Œï¼ŒæŸç§ç±»åž‹çš„æ‰€æœ‰pageçš„æ€»çš„ä¿¡æ¯ï¼Œå¦‚ï¼šsumï¼Œmaxï¼Œcountã€‚
>>>>>>> parent of bca47d5... å®žéªŒ
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

<<<<<<< HEAD
void Select_Data(unsigned int row,unsigned int column,short value)      // »ñÈ¡Ã¿¸ö½ÚµãÄÚ²¿µÄÐÅÏ¢
{
=======
void Select_Data(unsigned int row,unsigned int column,short value)
{// èŽ·å–æ¯ä¸ªèŠ‚ç‚¹å†…éƒ¨çš„ä¿¡æ¯
>>>>>>> parent of bca47d5... å®žéªŒ
    Word_Line_Node[row][column].count ++;
    Word_Line_Node[row][column].sum += value;
    if(Word_Line_Node[row][column].max < value)
        Word_Line_Node[row][column].max = value;
}

<<<<<<< HEAD
unsigned int c2h(unsigned char c)                                           //×Ö·û×°»»ÎªÊý×Ö
{
=======
unsigned int c2h(unsigned char c)
{//å­—ç¬¦è£…æ¢ä¸ºæ•°å­—
>>>>>>> parent of bca47d5... å®žéªŒ
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
<<<<<<< HEAD
unsigned int do_int(char *fp,int length)    //×Ö·û´®×ª»»Îª16½øÖÆÊý
{
=======
unsigned int do_int(char *fp,int length)
{ //å­—ç¬¦ä¸²è½¬æ¢ä¸º16è¿›åˆ¶æ•°
>>>>>>> parent of bca47d5... å®žéªŒ
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

<<<<<<< HEAD
void different_wordline(struct dirent * fileinfo)  // ÄÃ³öÄ¿Â¼ÖÐµÄrdberÎÄ¼þÐÅÏ¢Á÷
{
=======
void different_wordline(struct dirent * fileinfo)
{// æ‹¿å‡ºç›®å½•ä¸­çš„rdberæ–‡ä»¶ä¿¡æ¯æµ
>>>>>>> parent of bca47d5... å®žéªŒ
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

<<<<<<< HEAD
    fseek(fp,0,SEEK_END);                     //fpÖ¸ÏòÎÄ¼þÁ÷Ä©Î²
    file_length = ftell(fp);                 // ´ÓÎÄ¼þ¿ªÍ·µ½fpÖ¸ÏòµÄÎ»ÖÃÎÄ¼þ×Ö½ÚÊý
    fseek(fp,0,SEEK_SET);                    //fpÔÙ´ÎÖ¸ÏòÎÄ¼þ¿ªÍ·

    while(file_length > 0)
    {
        fread(&first_value,2,1,fp);              //¶ÁÈ¡Ò»¸öµ¥Ôª£¬Ã¿¸öµ¥ÔªÎªÁ½¸ö×Ö½Ú
        fread(&second_value,2,1,fp);

        GetWordLineNumber(page, &word_line_number, &page_type);      //»ñÈ¡wordlineºÅÒÔ¼°pageÀàÐÍ
        DistributionRowColumn(word_line_number, page_type, &row, &column);       //µÃµ½ËùÊôµÄÐÐºÅÁÐºÅ
        Select_Data(row, column , (first_value + second_value));           //  ´æ´¢Êý×éÔªËØÐÅÏ¢
=======
    fseek(fp,0,SEEK_END);//fpæŒ‡å‘æ–‡ä»¶æµæœ«å°¾
    file_length = ftell(fp);// ä»Žæ–‡ä»¶å¼€å¤´åˆ°fpæŒ‡å‘çš„ä½ç½®æ–‡ä»¶å­—èŠ‚æ•°
    fseek(fp,0,SEEK_SET);//fpå†æ¬¡æŒ‡å‘æ–‡ä»¶å¼€å¤´

    while(file_length > 0)
    {
        fread(&first_value,2,1,fp);//è¯»å–ä¸€ä¸ªå•å…ƒï¼Œæ¯ä¸ªå•å…ƒä¸ºä¸¤ä¸ªå­—èŠ‚
        fread(&second_value,2,1,fp);

        GetWordLineNumber(page, &word_line_number, &page_type);//èŽ·å–wordlineå·ä»¥åŠpageç±»åž‹
        DistributionRowColumn(word_line_number, page_type, &row, &column);//å¾—åˆ°æ‰€å±žçš„è¡Œå·åˆ—å·
        Select_Data(row, column , (first_value + second_value)); //å­˜å‚¨ç›¸åº”æ•°ç»„å…ƒç´ ä¿¡æ¯
>>>>>>> parent of bca47d5... å®žéªŒ


        codeword ++;
        if(codeword >= 4)
<<<<<<< HEAD
        {                                                     // ¾ö¶¨Ò³ºÅµÄÏÂÒÆ
=======
        {// å†³å®šé¡µå·çš„ä¸‹ç§»
>>>>>>> parent of bca47d5... å®žéªŒ
            page += 1;
            codeword = 0;
        }

<<<<<<< HEAD
        file_length = file_length - 4;                           // ¾ö¶¨ºÎÊ±¶ÁÍêÌø³öÎÄ¼þ
=======
        file_length = file_length - 4; // å†³å®šä½•æ—¶è¯»å®Œè·³å‡ºæ–‡ä»¶
>>>>>>> parent of bca47d5... å®žéªŒ
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

    sprintf(result_name,"%s/different_word_line/%s/different_word_line_result_%04d.txt",big_dir,dest_dir_name,endurance_number);
   // printf("55555555555555555555555555%s",result_name);
    chdir(result_name);
   printf("\n%s",result_name);
    if((result = fopen(result_name,"w+")) == NULL)
    {
        perror("\nfail to fopen ");
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
    // sprintf(dest_path,"%s/different_word_line",big_dir);

//    while((dir_file = readdir(dir)) != NULL)
//    {
//        if((dir_file->d_name[0] == dest_dir_name[0]) && (dir_file->d_name[1] == dest_dir_name[1]))
//           sprintf(dest_path,"%s/%s",dest_path,dir_file->d_name);
//    }
//     sprintf(command,"copy %s %s",result_name,dest_path);
    //printf("%s\n",command);
    //system(command);
    closedir(dir);
}

void statistics_a_directory(char *path,char *dest_dir_name,char*big_dir)
<<<<<<< HEAD
{                                                                               //1.rdberÎÄ¼þËùÔÚµÄÄ¿Â¼ £¬Èç£º\fifth_test\normal_tempurature_7\2_pattern\5055_4_0_0_0\all_nand
=======
{//1.rdberæ–‡ä»¶æ‰€åœ¨çš„ç›®å½• ï¼Œå¦‚ï¼š\fifth_test\normal_tempurature_7\2_pattern\5055_4_0_0_0\all_nand
>>>>>>> parent of bca47d5... å®žéªŒ
    DIR *dir = NULL;
    struct dirent * file_info = NULL;
    int count_number = 0;
    int endurance_number = 0;
<<<<<<< HEAD

    while(endurance_number <= 4)                                                    // ¸ù¾ÝEnduranceÏÈ½«block·ÖÀà£¬È»ºó
    {
        if((dir = opendir(path)) == NULL)                                       // ´ò¿ªÄ¿Â¼µÃµ½Ò»¸öÄ¿Â¼Á÷
=======
    while(endurance_number <= 4)// æ ¹æ®Enduranceå…ˆå°†blockåˆ†ç±»ï¼Œç„¶åŽ
    {
        if((dir = opendir(path)) == NULL)// æ‰“å¼€ç›®å½•å¾—åˆ°ä¸€ä¸ªç›®å½•æµ
>>>>>>> parent of bca47d5... å®žéªŒ
        {
            perror("fail to opendir");
            return -1;
        }
        do_init();

        count_number = 0;
        chdir(path);
<<<<<<< HEAD


        while((file_info = readdir(dir)) != NULL)                         // ¶ÁÈ¡Ä¿Â¼Á÷£¬µÃµ½ÎÄ¼þµÄÐÅÏ¢£¬È»ºóÏÂÒÆ
=======
        while((file_info = readdir(dir)) != NULL)// è¯»å–ç›®å½•æµï¼Œå¾—åˆ°æ–‡ä»¶çš„ä¿¡æ¯ï¼Œç„¶åŽä¸‹ç§»
>>>>>>> parent of bca47d5... å®žéªŒ
        {
            if(file_info->d_name[0] == '.')
                continue;
            if(strncmp(file_info->d_name,"rdber",5) != 0)
                continue;
           // printf("%s\n",file_info->d_name);
            UniqueId = do_int(file_info->d_name + 6,8);

            BlockNum = do_int(file_info->d_name + 15,4);

            Endurance = do_int(file_info->d_name + 20,4);

            if(Endurance != endurance_number)
<<<<<<< HEAD
                continue;                                                 //¸ù¾ÝEnduranceµÄ´ÎÐò´¦Àíblock
=======
                continue;//æ ¹æ®Enduranceçš„æ¬¡åºå¤„ç†block
>>>>>>> parent of bca47d5... å®žéªŒ

            different_wordline(file_info);
            count_number ++;
        }
<<<<<<< HEAD
       // printf("count_number:%d\n",count_number);
        printf("\n666666666%s",dest_dir_name);

        write_into_file(path,endurance_number,dest_dir_name,big_dir);    // ½«EnduranceÏàÍ¬µÄblock£¬·ÖÎöµÃµ½Êý¾Ý´æ´¢ÔÚÍ¬Ò»¸öÎÄ¼þÀï

        printf("%d finished!\n",endurance_number);
        endurance_number ++;                                    // ·ÖÎöÁíÍâµÄEnduranceÏÂµÄblock
        closedir(dir);                                                               // ¹Ø±ÕÄ¿Â¼Á÷
=======
        printf("count_number:%d\n",count_number);
        write_into_file(path,endurance_number,dest_dir_name,big_dir);
        // å°†Enduranceç›¸åŒçš„blockï¼Œåˆ†æžå¾—åˆ°æ•°æ®å­˜å‚¨åœ¨åŒä¸€ä¸ªæ–‡ä»¶é‡Œ
        printf("%d finished!\n",endurance_number);
        endurance_number ++;// åˆ†æžå¦å¤–çš„Enduranceä¸‹çš„block
        closedir(dir);  // å…³é—­ç›®å½•æµ
>>>>>>> parent of bca47d5... å®žéªŒ
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
    struct dirent *info_bake = NULL, *info_offset = NULL,search_dir_name[64] = {0};
    char bake_path[128] = {0},offset_path[128] = {0};
    int bake_name_length;
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
       bake_name_length = strlen (info_bake->d_name);
        strncpy(search_dir_name,info_bake->d_name + 11,bake_name_length - 11);//È¡info_bakeÀïÃæÃû×ÖµÄ´ÓµÚ11¸ö×Ö·û¿ªÊ¼µÄÁ½¸ö×Ö·û
        //printf("----------------%s\n",search_dir_name);
        if(info_bake->d_type != DT_DIR)
            continue;

        strcat(bake_path,"/");
        strcat(bake_path,info_bake->d_name);    // ×·¼ÓÄ¿Â¼

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
               printf("-------7-%s--7-----\n",offset_path);
            statistics_a_directory(offset_path,search_dir_name,argv[1]);

            memset(offset_path,0,sizeof(offset_path));//Ã¿Ò»´ÎÔËÐÐ½øÐÐÒ»´ÎÖÁÁã¡£

        }


        closedir(dir_offset);

        memset(bake_path,0,sizeof(bake_path));
        memset(search_dir_name,0,sizeof(search_dir_name));
    }

    closedir(dir_bake);
    //copy_result_to_dest_dir(argv[1]);


    return 0;
}
