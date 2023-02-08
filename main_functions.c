#include "minor_functions.c"

//word by word comprator

int input()
{
	FILE* cmd_file = fopen("system/cmd_file.cmd", "r");
	
	FILE* out_file=fopen("system/out_file.cmd","w");

    char cmd[cmd_max_len];
    char tmp[cmd_max_len];

    fscanf(cmd_file,"%s",cmd);

    if(strcmp(cmd,"createfile")==0)
    {
        fscanf(cmd_file,"%s",tmp);
        expected_cmd(tmp,"--file","expected --file.",-1);

        char path[str_max_len];
        int out=process_input(path,0,cmd_file);
        if(out==-1)
        {
            syntax_error("unexpected backslash.",-1);
        }
        createfile(path);
    }
    else if(strcmp(cmd,"insertstr")==0)
    {
        //get path
        fscanf(cmd_file,"%s",tmp);
        expected_cmd(tmp,"--file","expected --file.",-1);

        char path[str_max_len];
        int out=process_input(path,0,cmd_file);
        if(out==-1)
        {
            syntax_error("unexpected backslash.",-1);
        }
  
        //get string
        fscanf(cmd_file,"%s",tmp);
        expected_cmd(tmp,"--str","expected --str.",-1);

        char str[str_max_len];
        out=process_input(str,0,cmd_file);
        if(out==-1)
        {
            syntax_error("unexpected backslash.",-1);
        }

        //get position
        fscanf(cmd_file,"%s ",tmp);
        expected_cmd(tmp,"--pos","expected --pos.",-1);

        int line_index,char_index;
        fscanf(cmd_file,"%d:%d",&line_index,&char_index);
	
        int res=insertstr(path,str,line_index,char_index);
        if(res==0)
        {
        	save_file(path);
		}
    }
    else if(strcmp(cmd,"cat")==0)
    {
        fscanf(cmd_file,"%s",tmp);
        expected_cmd(tmp,"--file","expected --file.",-1);

        char path[str_max_len];
        int out=process_input(path,0,cmd_file);
        if(out==-1)
        {
            syntax_error("unexpected backslash.",-1);
        }

        int is_arman=0;
        while(out==1)
        {
            out=process_input(tmp,0,cmd_file);
            if(strcmp(tmp,"=D")==0)
            {
                is_arman=1;
                if(out==0)
                {
                    syntax_error("Arman command should be in one line.",-1);
                }
                break;
            }
        }

        if(is_arman==0)
        {
            cat(path,out_file);
        }
        else
        {
            FILE* ar_file=fopen("system/arman_output.txt","w");
            cat(path,ar_file);
            fclose(ar_file);

            arman(cmd_file);
        }

    }
    else if(strcmp(cmd,"removestr")==0 || strcmp(cmd,"copystr")==0 || strcmp(cmd,"cutstr")==0)
    {
        //get path
        fscanf(cmd_file,"%s",tmp);
        expected_cmd(tmp,"--file","expected --file.",-1);

        char path[str_max_len];
        int out=process_input(path,0,cmd_file);
        if(out==-1)
        {
            syntax_error("unexpected backslash.",-1);
        }

        //get position
        fscanf(cmd_file,"%s ",tmp);
        expected_cmd(tmp,"--pos","expected --pos.",-1);

        int line_index,char_index;
        fscanf(cmd_file,"%d:%d ",&line_index,&char_index);

        //get size
        fscanf(cmd_file,"%s ",tmp);
        expected_cmd(tmp,"-size","expected -size.",-1);

        int size;
        fscanf(cmd_file,"%d ",&size);

        //get flag
        int is_forward;
        char flag[5];
        fscanf(cmd_file,"%s",flag);

        is_forward= flag[1]=='f' ? 1 : 0;
        int res=-1;
        
        if(strcmp(cmd,"removestr")==0)
        {
            res=removestr(path,size,line_index,char_index,is_forward);
        }
        if(strcmp(cmd,"copystr")==0)
        {
            copystr(path,size,line_index,char_index,is_forward);
        }
        if(strcmp(cmd,"cutstr")==0)
        {
            res=copystr(path,size,line_index,char_index,is_forward);
            res+=removestr(path,size,line_index,char_index,is_forward);
        }
        if(res==0)
        {
        	save_file(path);
		}
    }
    else if(strcmp(cmd,"pastestr")==0)
    {
        fscanf(cmd_file,"%s",tmp);
        expected_cmd(tmp,"--file","expected --file.",-1);

        char path[str_max_len];
        int out=process_input(path,0,cmd_file);
        if(out==-1)
        {
            syntax_error("unexpected backslash.",-1);
        }

        //get position
        fscanf(cmd_file,"%s ",tmp);
        expected_cmd(tmp,"--pos","expected --pos.",-1);

        int line_index,char_index;
        fscanf(cmd_file,"%d:%d",&line_index,&char_index);

		
        int res=pastestr(path,line_index,char_index);
        if(res==0)
        {
        	save_file(path);
		}
    }
    else if(strcmp(cmd,"find")==0 || strcmp(cmd,"replace")==0)
    {
        //get string
        fscanf(cmd_file,"%s",tmp);
        if(strcmp(cmd,"find")==0)
        {
            expected_cmd(tmp,"--str","expected --str.",-1);
        }
        else
        {
            expected_cmd(tmp,"--str1","expected --str1.",-1);
        }

        char str[str_max_len];
        int out=process_input(str,1,cmd_file);
        if(out==-1)
        {
            syntax_error("unexpected backslash.",-1);
        }

        char str2[str_max_len];
        if(strcmp(cmd,"replace")==0)
        {
            fscanf(cmd_file,"%s",tmp);
            expected_cmd(tmp,"--str2","expected --str2.",-1);

            int out=process_input(str2,0,cmd_file);
            if(out==-1)
            {
                syntax_error("unexpected backslash.",-1);
            }
        }

        //get path
        fscanf(cmd_file,"%s",tmp);
        expected_cmd(tmp,"--file","expected --file.",-1);

        char path[str_max_len];
        out=process_input(path,0,cmd_file);
        if(out==-1)
        {
            syntax_error("unexpected backslash.",-1);
        }

        int count=0;
        int at=1;   //default first time
        int byword=0;
        int all=0;

        int is_arman=0;
        while(out==1)
        {
            out=process_input(tmp,0,cmd_file);
            if(strcmp(tmp,"-count")==0)
            {
                count=1;
            }
            else if(strcmp(tmp,"-at")==0)
            {
                int x;
                fscanf(cmd_file," %d",&x);
                at=x;
                char c=getchar();
                if(c==' ')
                {
                    out=1;
                }
                else if(c=='\n' || c==EOF)
                {
                    out=0;
                }
            }
            else if(strcmp(tmp,"-byword")==0)
            {
                byword=1;
            }
            else if(strcmp(tmp,"-all")==0)
            {
                all=1;
            }
            else if(strcmp(tmp,"=D")==0)
            {
                is_arman=1;
                if(out==0)
                {
                    syntax_error("Arman command should be in one line.",-1);
                }
                break;
            }
        }

        if(strcmp(cmd,"find")==0)
        {
            if(is_arman==0)
            {
                find(path,str,count,at,byword,all,out_file,0,"");
            }
            else
            {
                FILE* ar_file=fopen("system/arman_output.txt","w");
                find(path,str,count,at,byword,all,ar_file,0,"");
                fclose(ar_file);

                arman(cmd_file);
            }
        }
        else
        {
            if((count==1) || (byword==1) || (at>1 && all==1))
            {
                runtime_error("Invalid use of options.",-1);
            }
            
            
            int res=find(path,str,count,at,byword,all,out_file,1,str2);
            if(res==0)
	        {
	        	save_file(path);
			}
        }
    }
    else if(strcmp(cmd,"grep")==0)
    {
        fscanf(cmd_file,"%s",tmp);

        int flag=0; //0: none   1:  c   2: l
        if(strcmp(tmp,"-c")==0)
        {
            flag=1;
            fscanf(cmd_file,"%s",tmp);
        }
        else if(strcmp(tmp,"-l")==0)
        {
            flag=2;
            fscanf(cmd_file,"%s",tmp);
        }

        expected_cmd(tmp,"--str","expected --str.",-1);

        char str[str_max_len];
        int out=process_input(str,0,cmd_file);
        if(out==-1)
        {
            syntax_error("unexpected backslash.",-1);
        }

        fscanf(cmd_file,"%s",tmp);
        expected_cmd(tmp,"--files","expected --files.",-1);

        int files_count=0;
        char files[max_arguments_grep][str_max_len];

        int is_arman=0;
        while(out==1)
        {
            char path[str_max_len];
            out=process_input(path,0,cmd_file);
            if(out==-1)
            {
                syntax_error("unexpected backslash.",-1);
            }

            if(strcmp(path,"=D")==0)
            {
                is_arman=1;
                if(out==0)
                {
                    syntax_error("Arman command should be in one line.",-1);
                }
                break;
            }

            if(strlen(path)>0)
            {
                strcpy(files[files_count],path);
                files_count++;
            }
        }
        
        if(is_arman==0)
        {
            grep(str,files_count,files,flag,out_file);
        }
        else
        {
            FILE* ar_file=fopen("system/arman_output.txt","w");
            grep(str,files_count,files,flag,ar_file);
            fclose(ar_file);

            arman(cmd_file);
        }
    }
    else if(strcmp(cmd,"undo")==0)
    {
        fscanf(cmd_file,"%s",tmp);
        expected_cmd(tmp,"--file","expected --file.",-1);

        char path[str_max_len];
        int out=process_input(path,0,cmd_file);
        if(out==-1)
        {
            syntax_error("unexpected backslash.",-1);
        }

        undo(path);
    }
    else if(strcmp(cmd,"auto-indent")==0)
    {
        char path[str_max_len];
        int out=process_input(path,0,cmd_file);
        if(out==-1)
        {
            syntax_error("unexpected backslash.",-1);
        }
        
        
        int res=auto_indent(path);
        if(res==0)
        {
        	save_file(path);
		}
    }
    else if(strcmp(cmd,"compare")==0)
    {
        char path1[str_max_len];
        int out=process_input(path1,0,cmd_file);
        if(out==-1)
        {
            syntax_error("unexpected backslash.",-1);
        }

        char path2[str_max_len];
        out=process_input(path2,0,cmd_file);
        if(out==-1)
        {
            syntax_error("unexpected backslash.",-1);
        }

        int is_arman=0;
        while(out==1)
        {
            out=process_input(tmp,0,cmd_file);
            if(strcmp(tmp,"=D")==0)
            {
                is_arman=1;
                if(out==0)
                {
                    syntax_error("Arman command should be in one line.",-1);
                }
                break;
            }
        }

        if(is_arman==0)
        {
            compare(path1,path2,out_file);
        }
        else
        {
            FILE* ar_file=fopen("system/arman_output.txt","w");
            compare(path1,path2,ar_file);
            fclose(ar_file);

            arman(cmd_file);
        }
    }
    else if(strcmp(cmd,"tree")==0)
    {
        int depth;
        fscanf(cmd_file,"%d",&depth);

        if(depth<-1)
        {
            runtime_error("The depth should be at least -1.",-1);
        }

        int out;
        char c=fgetc(cmd_file);
        if(c=='\n' || c==EOF)
        {
            out=0;
        }
        else
        {
            out=1;
        }

        int is_arman=0;
        while(out==1)
        {
            out=process_input(tmp,0,cmd_file);
            if(strcmp(tmp,"=D")==0)
            {
                is_arman=1;
                if(out==0)
                {
                    syntax_error("Arman command should be in one line.",-1);
                }
                break;
            }
        }

        char c_path[str_max_len]="./root";
        char c_pre[str_max_len]="";

        if(is_arman==0)
        {
            fprintf(out_file,"root\n");
            tree(depth,0,c_path,c_pre,out_file);
        }
        else
        {
            FILE* ar_file=fopen("system/arman_output.txt","w");
            fprintf(ar_file,"root\n");
            tree(depth,0,c_path,c_pre,ar_file);
            fclose(ar_file);

            arman(cmd_file);
        }
    }
    else
    {
        syntax_error("Invalid command.",-1);
    }
    
    fclose(cmd_file);
    
    fclose(out_file);
    
    return 0;
}

int undo(char* path)
{
    path++; //first slash skipped
    if(file_existance(path)==-1)
    {
        runtime_error("This file doesn't exist.",-1);
    }
    else if(file_existance(path)==-2)
    {
        runtime_error("This directory doesn't exist.",-1);
    }

    int index=strlen(path)-1;
    while(1)
    {
    	if(path[index]=='/')
    	{
    		break;
		}
		index--;
	}
	
	char sys[str_max_len];
    sys[0]='\0';
    str_append(2,sys,path,".system_undo");
    
    str_middle_add(sys,".",index+1);

    if(file_existance(sys)==-1)
    {
        runtime_error("No changes to be undone. ",-1);
    }
    
    FILE* file=fopen(sys,"r");

    int last_undo_num;
    fscanf(file,"%d",&last_undo_num);
    if(last_undo_num==0)
    {
        runtime_error("No changes to be undone.",-1);
    }
    fclose(file);
    
    char num[max_undo];
    num[0]='\0';
    int_to_string(last_undo_num,num);

    char last_undo_file[str_max_len];
    str_append(2,last_undo_file,sys,num);

    copy(last_undo_file,path);

    file=fopen(sys,"w");
    fprintf(file,"%d",last_undo_num-1);
    fclose(file);

    remove(last_undo_file);

    return 0;
}

int save_file(char* path)
{
    path++; //first slash skipped
    if(file_existance(path)==-1)
    {
        runtime_error("This file doesn't exist.",-1);
    }
    else if(file_existance(path)==-2)
    {
        runtime_error("This directory doesn't exist.",-1);
    }
	
    int undo_size;
    
    int index=strlen(path)-1;
    while(1)
    {
    	if(path[index]=='/')
    	{
    		break;
		}
		index--;
	}
	
	char sys[str_max_len];
    sys[0]='\0';
    str_append(2,sys,path,".system_undo");
    
    str_middle_add(sys,".",index+1);

    FILE* file;
    if(file_existance(sys)==-1)
    {
        file=fopen(sys,"w");
        fprintf(file,"%d",0);
        undo_size=0;
    }
    else
    {
        file=fopen(sys,"r");
        fscanf(file,"%d",&undo_size);
    }
    fclose(file);
    file=fopen(sys,"w");

    undo_size++;

    char num[max_undo];
    int_to_string(undo_size,num);

    char new_undo_file[str_max_len];
    new_undo_file[0]='\0';
    str_append(2,new_undo_file,sys,num);

    copy("system/tmp.undo",new_undo_file);

    fprintf(file,"%d",undo_size);
    fclose(file);

    return 0;
}

int grep(char* str,int file_count,char files[][str_max_len],int flag,FILE* output_file)
{
    int has_str[file_count];
    clear_array(has_str,file_count);

    int len=strlen(str);

    for(int i=0;i<file_count;i++)
    {
        if(file_existance(files[i]+1)==-1)
        {
            runtime_error("This file doesn't exist.",-1);
        }
        else if(file_existance(files[i]+1)==-2)
        {
            runtime_error("This directory doesn't exist.",-1);
        }
        FILE* file=fopen(files[i]+1,"r");

        char line[line_max_len];
        while(feof(file)==0)
        {
            fgets(line,line_max_len,file);

            int line_len=strlen(line);

            int found=0;
            for(int j=0;j<line_len-len+1;j++)
            {
                char tmp_str[line_max_len];
                strcpy(tmp_str,line+j);
                tmp_str[len]='\0';

                if(strcmp(tmp_str,str)==0)
                {
                    found=1;
                    break;
                }
            }

            if(found==1)
            {
                if(flag==0)
                {
                    line[line_len-1]='\0';
                    fprintf(output_file,"%s : %s\n",files[i],line);
                }
                has_str[i]=1;
            }
        }
        fclose(file);
    }

    int cnt=0;
    for(int i=0;i<file_count;i++)
    {
        if(has_str[i]==1)
        {
            if(flag==1)
            {
                cnt++;
            }
            else if(flag==2)
            {
                fprintf(output_file,"%s\n",files[i]);
            }
        }
    }
    if(flag==1)
    {
        fprintf(output_file,"%d\n",cnt);
    }

    return 0;
}

int arman(FILE* cmd_file)
{
	FILE* out_file=fopen("system/out_file.cmd","w");
	
    char cmd[cmd_max_len];
    char tmp[cmd_max_len];

    int cmd_out=process_input(cmd,0,cmd_file);
    if(cmd_out==-1)
    {
        syntax_error("unexpected backslash.",-1);
    }

    if(strcmp(cmd,"insertstr")==0)
    {
        //get path
        fscanf(cmd_file,"%s",tmp);
        expected_cmd(tmp,"--file","expected --file.",-1);

        char path[str_max_len];
        int out=process_input(path,0,cmd_file);
        if(out==-1)
        {
            syntax_error("unexpected backslash.",-1);
        }

        //get string
        char str[str_max_len];
        get_text_of_file("system/arman_output.txt",str);

        //get position
        fscanf(cmd_file,"%s ",tmp);
        expected_cmd(tmp,"--pos","expected --pos.",-1);

        int line_index,char_index;
        fscanf(cmd_file,"%d:%d",&line_index,&char_index);

		
        int res=insertstr(path,str,line_index,char_index);
        if(res==0)
        {
        	save_file(path);
		}
    }
    else if(strcmp(cmd,"find")==0 || strcmp(cmd,"replace")==0)
    {
        char str[str_max_len];
        get_text_of_file("system/arman_output.txt",str);

        char str2[str_max_len];
        if(strcmp(cmd,"replace")==0)
        {
            fscanf(cmd_file,"%s",tmp);
            expected_cmd(tmp,"--str2","expected --str2.",-1);

            int out=process_input(str2,0,cmd_file);
            if(out==-1)
            {
                syntax_error("unexpected backslash.",-1);
            }
        }

        //get path
        fscanf(cmd_file,"%s",tmp);
        expected_cmd(tmp,"--file","expected --file.",-1);

        char path[str_max_len];
        int out=process_input(path,0,cmd_file);
        if(out==-1)
        {
            syntax_error("unexpected backslash.",-1);
        }

        int count=0;
        int at=1;   //default first time
        int byword=0;
        int all=0;

        int is_arman=0;
        while(out==1)
        {
            out=process_input(tmp,0,cmd_file);
            if(strcmp(tmp,"-count")==0)
            {
                count=1;
            }
            else if(strcmp(tmp,"-at")==0)
            {
                int x;
                fscanf(cmd_file," %d",&x);
                at=x;
                char c=getchar();
                if(c==' ')
                {
                    out=1;
                }
                else if(c=='\n')
                {
                    out=0;
                }
            }
            else if(strcmp(tmp,"-byword")==0)
            {
                byword=1;
            }
            else if(strcmp(tmp,"-all")==0)
            {
                all=1;
            }
            else if(strcmp(tmp,"=D")==0)
            {
                is_arman=1;
                if(out==0)
                {
                    syntax_error("Arman command should be in one line.",-1);
                }
                break;
            }
        }

        if(strcmp(cmd,"find")==0)
        {
            if(is_arman==0)
            {
                find(path,str,count,at,byword,all,out_file,0,"");
            }
            else
            {
                FILE* ar_file=fopen("system/arman_output.txt","w");
                find(path,str,count,at,byword,all,ar_file,0,"");
                fclose(ar_file);

                arman(cmd_file);
            }
        }
        else
        {
            if((count==1) || (byword==1) || (at>1 && all==1))
            {
                runtime_error("Invalid use of options.",-1);
            }
            
            int res=find(path,str,count,at,byword,all,out_file,1,str2);
            if(res==0)
	        {
	        	save_file(path);
			}
        }
    }
    else if(strcmp(cmd,"grep")==0)
    {
        fscanf(cmd_file,"%s",tmp);

        int flag=0; //0: none   1:  c   2: l
        if(strcmp(tmp,"-c")==0)
        {
            flag=1;
            fscanf(cmd_file,"%s",tmp);
        }
        else if(strcmp(tmp,"-l")==0)
        {
            flag=2;
            fscanf(cmd_file,"%s",tmp);
        }

        char str[str_max_len];
        get_text_of_file("system/arman_output.txt",str);

        fscanf(cmd_file,"%s",tmp);
        expected_cmd(tmp,"--files","expected --files.",-1);

        int files_count=0;
        char files[max_arguments_grep][str_max_len];

        int is_arman=0;
        int out=1;
        while(out==1)
        {
            char path[str_max_len];
            out=process_input(path,0,cmd_file);
            if(out==-1)
            {
                syntax_error("unexpected backslash.",-1);
            }

            if(strcmp(path,"=D")==0)
            {
                is_arman=1;
                if(out==0)
                {
                    syntax_error("Arman command should be in one line.",-1);
                }
                break;
            }

            if(strlen(path)>0)
            {
                strcpy(files[files_count],path);
                files_count++;
            }
        }
        
        if(is_arman==0)
        {
            grep(str,files_count,files,flag,out_file);
        }
        else
        {
            FILE* ar_file=fopen("system/arman_output.txt","w");
            grep(str,files_count,files,flag,ar_file);
            fclose(ar_file);

            arman(cmd_file);
        }
    }
    else
    {
        syntax_error("Invalid arman command",-1);
    }

	fclose(out_file);
	
    return 0;
}

int find(char* path,char* find_str, int count, int at, int byword, int all,FILE* output_file,int is_replace,char* replace_str)
{
    path++; //first slash skipped
    if(file_existance(path)==-1)
    {
        runtime_error("This file doesn't exist.",-1);
    }
    else if(file_existance(path)==-2)
    {
        runtime_error("This directory doesn't exist.",-1);
    }
    
    if(is_replace==1)
    {
    	copy(path,"system/tmp.undo");
	}

    if((count==1 && at>1) || (all==1 && at>1) || (count==1 && all==1))
    {
        runtime_error("Invalid use of multiple options.",-1);
    }

    char text_of_file[file_max_len];

    get_text_of_file(path, text_of_file);

    int len=strlen(text_of_file);
    int find_str_len=strlen(find_str);

    //for at and count flag
    int count_flag=0;

    //for byword flag
    int word_index=0;
    int has_word_started=0;

    //for all flag
    int previous_find=-1;

    for(int i=0;i<len;i++)
    {
        int check=1;

        if(text_of_file[i]==' ' || text_of_file[i]=='\n')
        {
            if(has_word_started==1)
            {
                word_index++;
                has_word_started=0;
            }
        }
        else
        {
            has_word_started=1;
        }

        int char_index=i;
        for(int j=0;j<find_str_len;j++)
        {
            if(i>=len)
            {
                check=0;
                break;
            }

            if(find_str[j]=='\\')
            {
                j++;
                if(text_of_file[char_index]!=find_str[j])
                {
                    check=0;
                    break;
                }
            }
            else if(find_str[j]=='*')
            {
                if(j==find_str_len-1 || find_str[j+1]==' ')
                {
                    while(1)
                    {
                        if(char_index==len || text_of_file[char_index]==' ')
                        {
                            break;
                        }
                        char_index++;
                    }
                    char_index--;
                }
                else if(j==0 || find_str[j-1]==' ')
                {
                    char wild_card_word[str_max_len];
                    char text_word[str_max_len];
                    j++;

                    //make wild_card_word
                    for(int k=0;k<str_max_len;k++)
                    {
                        wild_card_word[k]=find_str[j];
                        if(find_str[j]=='\\')
                        {
                            j++;
                            wild_card_word[k]=find_str[j];
                        }
                        if(j==find_str_len || find_str[j]==' ')
                        {
                            wild_card_word[k]='\0';
                            break;
                        }
                        j++;
                    }
                    j--;

                    //make text_word
                    for(int k=0;k<str_max_len;k++)
                    {
                        text_word[k]=text_of_file[char_index];
                        if(char_index==len || text_of_file[char_index]==' ')
                        {
                            text_word[k]='\0';
                            break;
                        }
                        char_index++;
                    }
                    char_index--;

                    int end_index=0;    //the last occurance of wild card word
                    int has_word=0;

                    int t_len=strlen(text_word);
                    int f_len=strlen(wild_card_word);
                    for(int k=0;k<t_len;k++)
                    {
                        int tmp_check=1;
                        int k_dup=k;
                        for(int h=0;h<f_len;h++)
                        {
                            if(k_dup==t_len)
                            {
                                tmp_check=0;
                                break;
                            }
                            if(wild_card_word[h]=='\\')
                            {
                                h++;
                            }
                            if(text_word[k_dup]!=wild_card_word[h])
                            {
                                tmp_check=0;
                                break;
                            }
                            k_dup++;
                        }
                        if(tmp_check==1)
                        {
                            has_word=1;
                            end_index=k_dup-1;
                        }
                    }

                    if(has_word==0)
                    {
                        check=0;
                        break;
                    }
                    else
                    {
                        char_index-=t_len-1-end_index;
                    }
                }
            }
            else
            {
                if(text_of_file[char_index]!=find_str[j])
                {
                    check=0;
                    break;
                }
            }
            char_index++;
        }

        if(check==1)
        {
            count_flag++;
            if(is_replace==1)
            {
                if(all==1)
                {
                    str_remove(text_of_file,i,char_index-1);
                    str_middle_add(text_of_file,replace_str,i);
                    len=strlen(text_of_file);
                }
                else if(at==count_flag)
                {
                    str_remove(text_of_file,i,char_index-1);
                    str_middle_add(text_of_file,replace_str,i);
                    break;
                }
            }
            else
            {
                if(count==1)
                {
                    continue;
                }
                else if(all==1)
                {
                    if(previous_find==-1)
                    {
                        previous_find= byword==1 ? word_index : i;
                    }
                    else
                    {
                        if(byword==1)
                        {
                            fprintf(output_file,"%d, ",previous_find);
                            previous_find=word_index;
                        }
                        else
                        {
                            fprintf(output_file,"%d, ",previous_find);
                            previous_find=i;
                        }
                    }
                }
                else if(at==count_flag)
                {
                    if(byword==1)
                    {
                        fprintf(output_file,"%d",word_index);
                    }
                    else
                    {
                        fprintf(output_file,"%d",i);
                    }
                    break;
                }
            }
        }
    }
    if(is_replace==1)
    {
        if(count_flag<at)
        {
            printf("The pattern not found.\n");
        }
        else
        {
            FILE* file=fopen(path,"w");
            fputs(text_of_file,file);
            fclose(file);
            printf("%d string(s) has been replaced.\n",count_flag);
        }
    }
    else
    {
        if(count==1)
        {
            fprintf(output_file,"%d",count_flag);
        }
        else if(count_flag<at)
        {

            fprintf(output_file,"-1");
        }
        else if(all==1)
        {
            fprintf(output_file,"%d",previous_find);
        }
    }

    return 0;
}

int compare(char* path1, char* path2, FILE* output_file)
{
    path1++; //first slash skipped
    if(file_existance(path1)==-1)
    {
        runtime_error("This file doesn't exist.",-1);
    }
    else if(file_existance(path1)==-2)
    {
        runtime_error("This directory doesn't exist.",-1);
    }

    path2++; //first slash skipped
    if(file_existance(path2)==-1)
    {
        runtime_error("This file doesn't exist.",-1);
    }
    else if(file_existance(path2)==-2)
    {
        runtime_error("This directory doesn't exist.",-1);
    }

    FILE* file1=fopen(path1,"r");
    FILE* file2=fopen(path2,"r");

    char line1[str_max_len];
    char line2[str_max_len];

    int line_index=1;
    while(feof(file1)==0 && feof(file2)==0)
    {
        fgets(line1,str_max_len,file1);
        fgets(line2,str_max_len,file2);

        if(line1[strlen(line1)-1]=='\n')
        {
            line1[strlen(line1)-1]='\0';
        }
        if(line2[strlen(line2)-1]=='\n')
        {
            line2[strlen(line2)-1]='\0';
        }

        if(strcmp(line1,line2)!=0)
        {
            fprintf(output_file,"============ #%d ============\n",line_index);
            fputs(line1,output_file);
            fputs(line2,output_file);
            fprintf(output_file,"\n");
        }

        line_index++;
    }

    if(feof(file1)==0)
    {
        int first_time=1;
        while(feof(file1)==0)
        {
            char* out=fgets(line1,str_max_len,file1);
            if(out==NULL)
            {
                break;
            }
            
            if(first_time==1)
            {
                fprintf(output_file,">>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>\n",line_index,line_index);
                first_time=0;
            }

            if(line1[strlen(line1)-1]=='\n')
            {
                line1[strlen(line1)-1]='\0';
            }
            fputs(line1,output_file);
        }
    }
    if(feof(file2)==0)
    {
        int first_time=1;
        while(feof(file2)==0)
        {
            char* out=fgets(line2,str_max_len,file2);
            if(out==NULL)
            {
                break;
            }

            if(first_time==1)
            {
                fprintf(output_file,"<<<<<<<<<<<< #%d - #%d <<<<<<<<<<<<\n",line_index,line_index);
                first_time=0;
            }

            if(line2[strlen(line2)-1]=='\n')
            {
                line2[strlen(line2)-1]='\0';
            }
            fputs(line2,output_file);
        }
    }

    fclose(file1);
    fclose(file2);

    return 0;
}

int auto_indent(char* path)
{
    path++; //first slash skipped
    if(file_existance(path)==-1)
    {
        runtime_error("This file doesn't exist.",-1);
    }
    else if(file_existance(path)==-2)
    {
        runtime_error("This directory doesn't exist.",-1);
    }
    copy(path,"system/tmp.undo");

    char text_of_file[file_max_len];
    get_text_of_file(path,text_of_file);

    char tab[10]="    "; //four spaces
    int current_indent_level=0;

    for(int i=0;i<str_max_len;i++)
    {
        if(text_of_file[i]=='\0')
        {
            break;
        }
        else if(text_of_file[i]=='{')
        {
            text_of_file[i]=' ';
            int prev_i=i;

            while(i>=0 && text_of_file[i]==' ')
            {
                i--;
            }

            if(i==-1 || (i>=0 && text_of_file[i]=='\n'))
            {
                text_of_file[prev_i]='{';
                if(i==-1)
                {
                    gap_remove(text_of_file,0);
                }
                i=prev_i;
                str_middle_add(text_of_file,"\n",i+1);
            }
            else
            {
                i++;    //on first space
                gap_remove(text_of_file,i);

                str_middle_add(text_of_file," {\n",i);
                i++;    //on {
            }

            current_indent_level++;
        }
        else if(text_of_file[i]=='\n')
        {
            gap_remove(text_of_file,i+1);
            for(int j=0;j<current_indent_level;j++)
            {
                str_middle_add(text_of_file,tab,i+1);
            }
        }
        else if(text_of_file[i]=='}')
        {
            if(current_indent_level>0)
            {
                current_indent_level--;
            }

            i--;
            while(i>=0 && (text_of_file[i]==' ' || text_of_file[i]=='\n'))
            {
                i--;
            }

            gap_remove(text_of_file,i+1);

            str_middle_add(text_of_file,"\n",i+1);
            i++;    //on \n

            for(int j=0;j<current_indent_level;j++)
            {
                str_middle_add(text_of_file,tab,i+1);
                i+=4;
            }
            i++;    //on }

            str_middle_add(text_of_file,"\n",i+1);
        }
    }

    FILE* file=fopen(path,"w");
    fputs(text_of_file,file);
    fclose(file);

    return 0;
}

int tree(int max_depth,int current_depth,char* current_path,char* pre_lines,FILE* output_file)
{
    if(current_depth==max_depth)
    {
        return 0;
    }

    DIR *d;
    struct dirent *dir;

    d = opendir(current_path);
    if(d==NULL)
    {
        runtime_error("There is no such folder",-1);
    }

    dir = readdir(d);
    while (dir!=NULL) 
    {
        if(strcmp(dir->d_name,"..")==0 || (dir->d_name)[0]=='.')
        {
            dir=readdir(d);
            continue;
        }

        fprintf(output_file,"%s",pre_lines);

        long int t=telldir(d);
        struct dirent *tmp_dir=dir;
        dir=readdir(d);
        while(dir!=NULL && (dir->d_name)[0]=='.')
        {
        	dir=readdir(d);
		}

        if(dir==NULL)   //next one is NULL
        {
            seekdir(d,t);   //back to previous
            dir=tmp_dir;
            fprintf(output_file,"L-------%s\n",dir->d_name);
            str_append(2,pre_lines,pre_lines," ");
            
        }
        else
        {
            seekdir(d,t);   //back to previous
            dir=tmp_dir;
            fprintf(output_file,"|-------%s\n",dir->d_name);
            str_append(2,pre_lines,pre_lines,"|");

        }
        

        char new_path[str_max_len];
        str_append(3,new_path,current_path,"/",dir->d_name);


        struct stat path_stat;
        stat(new_path, &path_stat);
        int is_file=S_ISREG(path_stat.st_mode);

        if(!is_file)  //is folder
        {
            tree(max_depth,current_depth+1,new_path,pre_lines,output_file);
        }

        pre_lines[strlen(pre_lines)-1]='\0';

        dir=readdir(d);
    }
    closedir(d);

    return 0;
}

int pastestr(char* path,int line_index,int char_index)
{
    char text_of_file[file_max_len];

    get_text_of_file("system/clipboard.txt",text_of_file);

    return insertstr(path,text_of_file,line_index,char_index);
}

int copystr(char* path,int size,int line_index,int char_index,int is_forward)
{   
    if(size==0)
    {
        return 0;
    }

    path++; //first slash skipped
    if(file_existance(path)==-1)
    {
        runtime_error("This file doesn't exist.",-1);
    }
    else if(file_existance(path)==-2)
    {
        runtime_error("This directory doesn't exist.",-1);
    }

    FILE* file=fopen(path,"r");
    char text_of_file[file_max_len];
    text_of_file[0]='\0';

    for(int i=0;i<line_index-1;i++)
    {
        char inp[file_max_len];
        if(fgets(inp,file_max_len,file)==NULL)
        {
            runtime_error("This position is invalid.",-1);
        }
        if(feof(file)!=0)
        {
            runtime_error("This position is invalid.",-1);
        }
        str_append(2,text_of_file,text_of_file,inp);
    }

    char tmp[file_max_len];
    tmp[0]='\0';

    for(int i=0;i<char_index;i++)
    {
        tmp[i]=fgetc(file);
        if(tmp[i]==EOF || tmp[i]=='\n')
        {
            runtime_error("This position is invalid.",-1);
        }
    }
    tmp[char_index]='\0';
    
    str_append(2,text_of_file,text_of_file,tmp);
    int len=strlen(text_of_file);

    //get rest of file
    while(1)
    {
        char inp[file_max_len];
        if(fgets(inp,file_max_len,file)==NULL)
        {
            break;
        }

        str_append(2,text_of_file,text_of_file,inp);
    }
    fclose(file);

    //remove part of text

    int finel_len=strlen(text_of_file);

    if(is_forward==0)
    {
        if(len-size<0)
        {
            runtime_error("invalid size of remove.",-1);
        }
        len-=size;
    }

    if(len+size>finel_len)
    {
        runtime_error("invalid size of remove.",-1);
    }
    
    text_of_file[len+size]='\0';
    char cpy[str_max_len];

    strcpy(cpy,text_of_file+len);

    file=fopen("system/clipboard.txt","w");
    fputs(cpy,file);
    fclose(file);

    return 0;
}

int removestr(char* path,int size,int line_index,int char_index,int is_forward)
{   
    if(size==0)
    {
        return 0;
    }

    path++; //first slash skipped
    if(file_existance(path)==-1)
    {
        runtime_error("This file doesn't exist.",-1);
    }
    else if(file_existance(path)==-2)
    {
        runtime_error("This directory doesn't exist.",-1);
    }
    
    copy(path,"system/tmp.undo");

    FILE* file=fopen(path,"r");
    char text_of_file[file_max_len];
    text_of_file[0]='\0';

    for(int i=0;i<line_index-1;i++)
    {
        char inp[file_max_len];
        if(fgets(inp,file_max_len,file)==NULL)
        {
            runtime_error("This position is invalid.",-1);
        }
        if(feof(file)!=0)
        {
            runtime_error("This position is invalid.",-1);
        }
        str_append(2,text_of_file,text_of_file,inp);
    }

    char tmp[file_max_len];
    tmp[0]='\0';

    for(int i=0;i<char_index;i++)
    {
        tmp[i]=fgetc(file);
        if(tmp[i]==EOF || tmp[i]=='\n')
        {
            runtime_error("This position is invalid.",-1);
        }
    }
    tmp[char_index]='\0';
    
    str_append(2,text_of_file,text_of_file,tmp);
    int len=strlen(text_of_file);

    //get rest of file
    while(1)
    {
        char inp[file_max_len];
        if(fgets(inp,file_max_len,file)==NULL)
        {
            break;
        }

        str_append(2,text_of_file,text_of_file,inp);
    }
    fclose(file);

    //remove part of text

    int finel_len=strlen(text_of_file);

    if(is_forward==0)
    {
        if(len-size<0)
        {
            runtime_error("invalid size of remove.",-1);
        }
        len-=size;
    }

    if(len+size>finel_len)
    {
        runtime_error("invalid size of remove.",-1);
    }
    
    text_of_file[len]='\0';
    str_append(2,text_of_file,text_of_file,text_of_file+len+size);

    file=fopen(path,"w");
    fputs(text_of_file,file);
    fclose(file);

    return 0;
}

int cat(char* path,FILE* output_file)
{
    path++; //first slash skipped
    if(file_existance(path)==-1)
    {
        runtime_error("This file doesn't exist.",-1);
    }
    else if(file_existance(path)==-2)
    {
        runtime_error("This directory doesn't exist.",-1);
    }

    char text_of_file[file_max_len];
    get_text_of_file(path,text_of_file);

    fputs(text_of_file,output_file);

    return 0;
}

int insertstr(char* path,char* str,int line_index,int char_index)
{   
    path++; //first slash skipped
    if(file_existance(path)==-1)
    {
        runtime_error("This file doesn't exist.",-1);
    }
    else if(file_existance(path)==-2)
    {
        runtime_error("This directory doesn't exist.",-1);
    }
    copy(path,"system/tmp.undo");

    FILE* file=fopen(path,"r");
    char text_of_file[file_max_len];
    text_of_file[0]='\0';
	
    for(int i=0;i<line_index-1;i++)
    {
        char inp[file_max_len];
        if(fgets(inp,file_max_len,file)==NULL)
        {
            runtime_error("This position is invalid.",-1);
        }
        if(feof(file)!=0)
        {
            runtime_error("This position is invalid.",-1);
        }
        str_append(2,text_of_file,text_of_file,inp);
    }
    	
    char tmp[file_max_len];
    tmp[0]='\0';

    for(int i=0;i<char_index;i++)
    {
        tmp[i]=fgetc(file);
        if(tmp[i]==EOF || tmp[i]=='\n')
        {
            runtime_error("This position is invalid.",-1);
        }
    }

    tmp[char_index]='\0';
    
    str_append(3,text_of_file,text_of_file,tmp,str);

    //get rest of file
    while(1)
    {
        char inp[file_max_len];
        if(fgets(inp,file_max_len,file)==NULL)
        {
            break;
        }

        str_append(2,text_of_file,text_of_file,inp);
    }
    fclose(file);

    file=fopen(path,"w");
    fputs(text_of_file,file);
    fclose(file);

    return 0;
}

int createfile(char* path)
{
    char folder_name[folder_name_max_len];
    int folder_name_index=0;
    char current_path[str_max_len];

    path++; //first slash skipped
    for(int i=0;path[i]!='\0';i++)
    {
        current_path[i]=path[i];
        folder_name[folder_name_index]=path[i];
        if(path[i]=='/')
        {
            folder_name[folder_name_index]='\0';
            folder_name_index=-1;
            
            if(folder_name[0]==' ')
            {
                syntax_error("invalid folder name",-1);
            }

            if(strcmp(current_path,"root/")==0)
            {
                folder_name_index++;
                continue;
            }

            current_path[i]='\0';
            mkdir(current_path,0777);
            current_path[i]='/';
        }
        folder_name_index++;
    }

    FILE* new_file=fopen(path, "r");
    if(new_file!=NULL)
    {
        runtime_error("This file exists.",-1);
    }
    new_file=fopen(path, "w");
    fclose(new_file);

    return 0;
}

void index_to_pos(char* str,int index,int* pos_y,int* pos_x)
{
	int len=strlen(str);
	int x=0;
	int y=0;
	
	for(int i=0;i<index;i++)
	{
		if(str[i]=='\n')
		{
			y++;
			x=0;
		}
		else
		{
			x++;
		}
	}
	
	*pos_y=y;
	*pos_x=x;
}
