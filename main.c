#include "main_functions.c"

//curser end of line
//convert printf to addstr

int main()
{
	initscr();
	noecho();
	
	int mode = 1;	//1: normal		2: edit		3: visual
	int edit_pos_x = 0;
	int edit_pos_y = 0;

	int visual_start_pos_x;
	int visual_start_pos_y;

	int win_height, win_width;

	char path[str_max_len];

	strcpy(path, "system/unsaved.unsaved");
	
	FILE* file = fopen(path, "w");
	fclose(file);

	char file_name[str_max_len]="untitled.txt";
	
	char text_of_file[str_max_len] = "\0";

	int normal_command_started = 0;
	char normal_command[str_max_len];

	getmaxyx(stdscr, win_height, win_width);
	move(win_height - 1, 0);

	int start_line_index = 0;
	
	int is_saved=0;
	
	int is_find=0;
	int find_count;
	int find_at;
	int find_highlighted=0;
	char find_str[str_max_len];

	while (1)
	{
		int c = getch();

		nodelay(stdscr, 1);
		int tmp_char = getch();
		nodelay(stdscr, 0);

		int x, y;
		getmaxyx(stdscr, win_height, win_width);

		if (mode == 1)	// normal
		{
			if(c!='n')
			{
				find_highlighted=0;
			}
			else if(find_highlighted==1)
			{
				if(find_at==find_count)
				{
					find_at=1;
				}
				else
				{
					find_at++;
				}
				FILE* tm_file=fopen("system/tmp_find.cmd","w");
				fputs(text_of_file,tm_file);
				fclose(tm_file);
					
				FILE* fn_fl=fopen("system/fnfl.txt","w");
				find("/system/tmp_find.cmd",find_str,0,find_at,0,0,fn_fl,0,"");
				fclose(fn_fl);
				
				int index;
				
				fn_fl=fopen("system/fnfl.txt","r");
				fscanf(fn_fl,"%d",&index);
				fclose(fn_fl);
				
				index_to_pos(text_of_file,index,&edit_pos_y,&edit_pos_x);
				
				goto_pos(text_of_file,&edit_pos_y,&edit_pos_x,&start_line_index);
			}
			
			if (normal_command_started == 1)
			{
							
				if (c == '\n')
				{
					//clear normal line
					move(win_height - 1, 0);
					for (int i = 0; i <= win_width; i++)
					{
						addch(' ');
					}
										
					move(win_height - 1, 0);
					
					if(is_find==1)
					{
						FILE* tm_file=fopen("system/tmp_find.cmd","w");
						fputs(text_of_file,tm_file);
						fclose(tm_file);
						
						tm_file=fopen("system/tmp_find2.tmp","w");
						find("/system/tmp_find.cmd",normal_command,1,1,0,0,tm_file,0,"");
						fclose(tm_file);
						
						find_highlighted=1;
						
						tm_file=fopen("system/tmp_find2.tmp","r");
						fscanf(tm_file,"%d",&find_count);
						fclose(tm_file);
						
						strcpy(find_str,normal_command);
						
						find_at=1;

						FILE* fn_fl=fopen("system/fnfl.txt","w");
						find("/system/tmp_find.cmd",find_str,0,find_at,0,0,fn_fl,0,"");
						fclose(fn_fl);
						
						int index;
						
						fn_fl=fopen("system/fnfl.txt","r");
						fscanf(fn_fl,"%d",&index);
						fclose(fn_fl);
						
						if(find_count>0)
						{
							index_to_pos(text_of_file,index,&edit_pos_y,&edit_pos_x);
						
							goto_pos(text_of_file,&edit_pos_y,&edit_pos_x,&start_line_index);
						}
						else
						{
							find_highlighted=0;
						}
					}
					else
					{
						run_command(normal_command, path, text_of_file,file_name,&is_saved,&edit_pos_x,&edit_pos_y);
					}

					normal_command_started = 0;
					normal_command[0] = '\0';
					
					is_find=0;
				}
				else
				{
					x = strlen(normal_command);

					if (x < win_width - 1)
					{
						normal_command[x] = c;
						normal_command[x + 1] = '\0';
						addch(c);
					}
				}
			}
			else
			{
				if (c == ':' || c == '/')
				{
					//clear normal line
					move(win_height - 1, 0);
					for (int i = 0; i <= win_width; i++)
					{
						addch(' ');
					}

					move(win_height - 1, 0);
					addch(c);
					normal_command_started = 1;
					
					if(c=='/')
					{
						is_find=1;
					}
				}
				else if (c == 'i' && normal_command_started == 0)
				{
					mode = 2;
					goto_pos(text_of_file, &edit_pos_y, &edit_pos_x, &start_line_index);
				}
				else if (c == 'v' && normal_command_started == 0)
				{
					mode = 3;
					goto_pos(text_of_file, &edit_pos_y, &edit_pos_x, &start_line_index);
					visual_start_pos_x = edit_pos_x;
					visual_start_pos_y = edit_pos_y;
				}
				else if (c == 'p' && normal_command_started == 0)
				{				
					FILE* fl=fopen("system/tmp_for_cmd.cmd","w");
					fputs(text_of_file,fl);
					fclose(fl);
					
					char path1[str_max_len]="/system/tmp_for_cmd.cmd";
					
					pastestr(path1,edit_pos_y+1,edit_pos_x);
					
					get_text_of_file("system/tmp_for_cmd.cmd",text_of_file);
					
					char paste_str[str_max_len];
					get_text_of_file("system/clipboard.txt",paste_str);
					int slen=strlen(paste_str);
					
					for(int i=0;i<slen;i++)
					{
						if (paste_str[i] == '\n')
						{
							edit_pos_x = 0;
							edit_pos_y++;
						}
						else
						{
							edit_pos_x++;
						}
					}
					goto_pos(text_of_file, &edit_pos_y, &edit_pos_x, &start_line_index);
				}
				else if (c == '=' && normal_command_started == 0)
				{
					FILE* fl=fopen("system/tmp_for_cmd.cmd","w");
					fputs(text_of_file,fl);
					fclose(fl);
					
					char path1[str_max_len]="/system/tmp_for_cmd.cmd";
					auto_indent(path1);
					
					get_text_of_file("system/tmp_for_cmd.cmd",text_of_file);
					edit_pos_y=count_lines_of_file(text_of_file)-1;
					edit_pos_x=chars_in_line(text_of_file,edit_pos_y);
					
					goto_pos(text_of_file, &edit_pos_y, &edit_pos_x, &start_line_index);
				}
				else if (c == 'u' && normal_command_started == 0)
				{
					char tmp_path[str_max_len]="\0";
					str_append(2,tmp_path,"/",path);
					
					undo(tmp_path);
					
					is_saved=1;
					get_text_of_file(path,text_of_file);
					
					edit_pos_y=count_lines_of_file(text_of_file)-1;
					edit_pos_x=chars_in_line(text_of_file,edit_pos_y);
					
					goto_pos(text_of_file, &edit_pos_y, &edit_pos_x, &start_line_index);
				}
			}

		}
		else if (mode == 2)	// edit
		{
			if (c == KEY_ESC && tmp_char == -1)
			{
				mode = 1;
				//getyx(stdscr, edit_pos_y, edit_pos_x);	//save edit mode position
			}
			else if (c < 128)
			{
				is_saved=0;
				int index = pos_to_index(text_of_file, edit_pos_y, edit_pos_x);
				FILE* fo = fopen("deb.txt", "w");
				fprintf(fo, "%d %d %d", edit_pos_y, edit_pos_x,index);
				fclose(fo);

				char st_char[5];
				st_char[0] = c;
				st_char[1] = '\0';
				str_middle_add(text_of_file, st_char, index);

				if (c == '\n')
				{
					edit_pos_x = 0;
					edit_pos_y++;
				}
				else
				{
					edit_pos_x++;
					addch(c);
				}
			}

			goto_pos(text_of_file, &edit_pos_y, &edit_pos_x, &start_line_index);
		}
		else if (mode == 3)	// visual
		{
			getyx(stdscr, y, x);
			int len = strlen(text_of_file);

			if (c == 'i')
			{
				mode = 2;
			}
			else if (c == KEY_ESC && tmp_char == -1)
			{
				mode = 1;
				//getyx(stdscr, edit_pos_y, edit_pos_x);	//save edit mode position
			}
			else if (c == 'l')
			{
				edit_pos_x++;
			}
			else if (c == 'h')
			{
				if (edit_pos_x > 0)
				{
					edit_pos_x--;
				}
			}
			else if (c == 'j')
			{
				edit_pos_y++;
			}
			else if (c == 'k')
			{
				if (edit_pos_y > 0)
				{
					edit_pos_y--;
				}
			}
			else if (c == 'y' || c=='d')	//copy or cut
			{
				FILE* fl=fopen("system/tmp_for_cmd.cmd","w");
				fputs(text_of_file,fl);
				fclose(fl);
				
				int index1=pos_to_index(text_of_file,visual_start_pos_y,visual_start_pos_x);
				int index2=pos_to_index(text_of_file,edit_pos_y,edit_pos_x);
				int size=index2-index1;
				
				char path[str_max_len]="/system/tmp_for_cmd.cmd";
				if(size>0)
				{
					copystr(path,size,visual_start_pos_y+1,visual_start_pos_x,1);
				}
				else
				{
					copystr(path,size,edit_pos_y+1,edit_pos_x,1);
				}
				
				if (c == 'd')	//cut
				{
					if(size>0)
					{
						removestr(path,size,visual_start_pos_y+1,visual_start_pos_x,1);
					}
					else
					{
						removestr(path,-size,edit_pos_y+1,edit_pos_x,1);
					}
					get_text_of_file("system/tmp_for_cmd.cmd",text_of_file);
					
					if(size>0)
					{
						edit_pos_y=visual_start_pos_y;
						edit_pos_x=visual_start_pos_x;
					}
				}
			}
			goto_pos(text_of_file, &edit_pos_y, &edit_pos_x, &start_line_index);
		}

		show_lines(text_of_file, start_line_index, win_height);
		show_footer_line(mode, file_name, is_saved);
		
		getyx(stdscr,y,x);
		if(find_highlighted==1)
		{
			for(int i=1;i<=find_count;i++)
			{
				FILE* tm_file=fopen("system/tmp_find.cmd","w");
				fputs(text_of_file,tm_file);
				fclose(tm_file);
				
				int posx,posy;
				
				FILE* fn_fl=fopen("system/fnfl.txt","w");
				find("/system/tmp_find.cmd",find_str,0,i,0,0,fn_fl,0,"");
				fclose(fn_fl);
				
				int index;
				
				fn_fl=fopen("system/fnfl.txt","r");
				fscanf(fn_fl,"%d",&index);
				fclose(fn_fl);
				
				index_to_pos(text_of_file,index,&posy,&posx);
				
				int st_dup=start_line_index;
				goto_pos(text_of_file,&posy,&posx,&st_dup);
				
				start_color();
				init_pair(5,0,7);
				attron(COLOR_PAIR(5));
				for(int j=index;j<index+strlen(find_str);j++)
				{
					addch(text_of_file[j]);
				}
				attroff(COLOR_PAIR(5));
			}
		}
		move(y,x);

		refresh();
	}
	endwin();
}

int run_command(char* str,char* path,char* text_of_file,char* file_name,int* is_saved,int* edit_pos_x,int* edit_pos_y)
{
	int x, y;
	getyx(stdscr, y, x);

	FILE* cmd_file = fopen("system/cmd_file.cmd", "w");
	fputs(str,cmd_file);
	fclose(cmd_file);

	cmd_file = fopen("system/cmd_file.cmd", "r");

	char cmd[str_max_len];
	fscanf(cmd_file, "%s", cmd);

	int width, height;
	getmaxyx(stdscr, height, width);

	move(height - 1, 0);

	if (strcmp(cmd, "save") == 0)
	{
		if (path[0] == 's')	//it is at system directory
		{
			runtime_error("You should first specify a directory and a name for your file. ", -1);
		}

		FILE* file = fopen(path, "w");
		fputs(text_of_file, file);
		fclose(file);

		char tmp_path[str_max_len]="\0";
		str_append(2,tmp_path,"/",path);
		
		copy(path,"system/tmp.undo");
		save_file(tmp_path);
		
		*is_saved=1;
		successful("File changes saved.");
	}
	else if (strcmp(cmd, "saveas") == 0)
	{
		char save_path[str_max_len];
		int out = process_input(save_path, 0, cmd_file);
		if (out == -1)
		{
			syntax_error("unexpected backslash.", -1);
		}

		if (file_existance(save_path + 1) == 0)
		{
			runtime_error("This file already exists.", -1);
		}
		else if (file_existance(save_path + 1) == -2)
		{
			runtime_error("This directory doesn't exist.", -1);
		}

		FILE* file = fopen(save_path + 1, "w");
		fputs(text_of_file, file);
		fclose(file);
		
		copy(save_path+1,"system/tmp.undo");
		save_file(save_path);
		
		strcpy(path, save_path + 1);

		//finding name of file
		int index=strlen(save_path)-1;
		while(1)
		{
			if(save_path[index]=='/')
			{
				break;
			}
			index--;
		}
		
		strcpy(file_name,save_path+index+1);
		
		*is_saved=1;
		successful("File changes saved.");
	}
	else if (strcmp(cmd, "open") == 0)
	{
		/*if (path[0] == 's')	//it is at system directory
		{
			runtime_error("This file hasn't been saved.", -1);
		}*/

		FILE* file = fopen(path, "w");
		fputs(text_of_file, file);
		fclose(file);

		char open_path[str_max_len];
		int out = process_input(open_path, 0, cmd_file);
		if (out == -1)
		{
			syntax_error("unexpected backslash.", -1);
		}

		if (file_existance(open_path + 1) == -1)
		{
			runtime_error("This file doesn't exist.", -1);
		}
		else if (file_existance(open_path + 1) == -2)
		{
			runtime_error("This directory doesn't exist.", -1);
		}
		
		//finding name of file
		int index=strlen(open_path)-1;
		while(1)
		{
			if(open_path[index]=='/')
			{
				break;
			}
			index--;
		}
		
		char tmp_path[str_max_len]="\0";
		str_append(2,tmp_path,"/",path);
		
		copy(path,"system/tmp.undo");
		save_file(tmp_path);
		
		strcpy(file_name,open_path+index+1);

		strcpy(path, open_path + 1);
		
		*is_saved=1;
		get_text_of_file(path, text_of_file);
	}
	else if(strcmp(cmd, "undo") == 0 && strlen(str)==4)
	{
		char tmp_path[str_max_len]="\0";
		str_append(2,tmp_path,"/",path);
		
		undo(tmp_path);
		
		*is_saved=1;
		get_text_of_file(path,text_of_file);
		
		*edit_pos_y=count_lines_of_file(text_of_file)-1;
		*edit_pos_x=chars_in_line(text_of_file,*edit_pos_y);
	}
	else
	{
		FILE* fil=fopen("system/out_file.cmd","w");
		fclose(fil);
		
		input();
		
		if(strcmp(cmd, "grep") == 0 || strcmp(cmd, "find") == 0 ||
		strcmp(cmd, "compare") == 0 || strcmp(cmd, "tree") == 0 || strcmp(cmd, "cat") == 0)
		{
			if (path[0] != 's')	//it is not at system directory
			{
				FILE* file = fopen(path, "w");
				fputs(text_of_file, file);
				fclose(file);
		
				char tmp_path[str_max_len]="\0";
				str_append(2,tmp_path,"/",path);
				
				copy(path,"system/tmp.undo");
				save_file(tmp_path);
			}
			
			get_text_of_file("system/out_file.cmd",text_of_file);
			
			*is_saved=0;
			
			*edit_pos_y=count_lines_of_file(text_of_file)-1;
			*edit_pos_x=chars_in_line(text_of_file,*edit_pos_y);
		}
	}

	fclose(cmd_file);

	move(y, x);

	return 0;
}

void show_footer_line(int mode, char* file_name, int is_saved)
{
	if (has_colors() == FALSE) {

		exit(1);
	}

	int x, y, height, width;
	getyx(stdscr, y, x);
	getmaxyx(stdscr, height, width);

	move(height - 2, 0);

	start_color();
	init_pair(1, 0, 81);
	attron(COLOR_PAIR(1));

	if (mode == 1)
	{
		addstr(" NORMAL ");
	}
	else if (mode == 2)
	{
		addstr("  EDIT  ");
	}
	else
	{
		addstr(" VISUAL ");
	}

	attroff(COLOR_PAIR(1));

	init_pair(2, 15, 238);

	attron(COLOR_PAIR(2));

	int len = strlen(file_name);
	if (len > 10)
	{
		addch(' ');
		for (int i = 0; i < 7; i++)
		{
			addch(file_name[i]);
		}
		addstr("... ");
	}
	else
	{
		addch(' ');
		addstr(file_name);
		addch(' ');
	}

	if (is_saved == 0)
	{
		addstr("+ ");
	}
	else
	{
		addstr("  ");
	}

	attroff(COLOR_PAIR(2));

	init_pair(3, 0, 236);

	attron(COLOR_PAIR(3));

	for (int i = 22; i < width; i++)
	{
		addch(' ');
	}

	attroff(COLOR_PAIR(3));

	move(y, x);
}

void show_lines(char* text, int st_index, int line_count)
{
	int x_pos, y_pos;
	getyx(stdscr, y_pos, x_pos);
	move(0, 0);

	int maxx, maxy;
	getmaxyx(stdscr, maxy, maxx);

	int file_line_count = count_lines_of_file(text);

	FILE* file = fopen("system/tmp.unsaved", "w");
	fputs(text, file);
	fclose(file);

	int y = 0;
	file = fopen("system/tmp.unsaved", "r");
	int file_ended = 1;

	for (int i = 0; i < file_line_count; i++)
	{
		char line[str_max_len];
		line[0] = '\0';
		fgets(line, file_max_len, file);

		if (i >= st_index)
		{

			y += ceiling((chars_in_line(text, i) + 5), maxx);
			if (y < maxy - 1)
			{
				/*FILE* num = fopen("system/num.cmd","w");
				fprintf(num, " %3d %s", i + 1, line);
				fclose(num);
				num = fopen("system/num.cmd", "r");
				
				char st[str_max_len];
				fgets(st, str_max_len, num);

				fclose(num);

				addstr(st);&*/
				printw(" %3d %s", i + 1, line);
			}
			else
			{
				file_ended = 0;
				y -= ceiling((chars_in_line(text, i) + 5), maxx);
				break;
			}
		}
	}

	if (file_ended == 1)
	{
		addstr("\n");
		for (int i = 0; i < maxy - 2 - y; i++)
		{
			addstr("~\n");
		}
	}
	
	move(y_pos, x_pos);
	fclose(file);
}

void goto_pos(char* text_of_file, int* pos_y, int* pos_x, int* st_line)
{
	if (*pos_y < *st_line + 3)
	{
		*st_line = max2(0, *pos_y - 3);
	}

	int line_count = count_lines_of_file(text_of_file);
	if (*pos_y >= line_count)
	{
		*pos_y = line_count - 1;
		return;
	}

	int c_in_l = chars_in_line(text_of_file, *pos_y);

	if (*pos_x > c_in_l)
	{
		*pos_x = c_in_l;
	}

	int maxx, maxy;
	getmaxyx(stdscr, maxy, maxx);

	int y = 0;
	for (int i = *st_line; i < *pos_y; i++)
	{
		int c = chars_in_line(text_of_file, i);
		c += 5;	//start of line
		y += ceiling(c, maxx);

		while (y >= maxy - 5)
		{
			int c2 = chars_in_line(text_of_file, *st_line);
			c2 += 5;	//start of line
			y -= ceiling(c2, maxx);
			(*st_line)++;
		}
	}

	int x = *pos_x;
	x += 5;
	y += ceiling(x, maxx) - 1;
	
	move(y, x % maxx);
}



