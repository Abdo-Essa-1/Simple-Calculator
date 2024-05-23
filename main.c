#include <avr/io.h>
#include <util/delay.h>
#include "common_macros.h"
#include "gpio.h"
#include "std_types.h"
#include "keypad.h"
#include "lcd.h"

int powerr10(uint8 n)
{
	int num = 1;
	while(n>0)
	{
		num *= 10;
		--n;
	}
	return num;
}

int main()
{
	/******Initializations******/
	LCD_init();
	int data[32], cursor , key, row, col, ans = 0;

	/******PROGRAM******/
	while(1)
	{
		for(cursor = 0; cursor < 32; ++cursor)
		{
			if(cursor < 16)
			{
				row = 0;
				col = cursor;
			}
			else
			{
				row = 1;
				col = cursor - 16;
			}
			_delay_ms(500);
			key = KEYPAD_getPressedKey();
			if(key == 'c')
			{
				LCD_clearScreen();
				_delay_ms(10);
				break;
			}
			else if(key == '=')
			{
				int num1 = 0, num2 = 0, index = 0;
				data[cursor] = key;
				LCD_moveCursor(row, col);
				_delay_ms(10);
				LCD_displayCharacter(data[cursor]);
				_delay_ms(1000);
				LCD_clearScreen();
				_delay_ms(10);
				if(0 > data[0] || data[0] > 9)
				{
					index = 1;
					uint8 digits = 0;
					while(0 <= data[index] && data[index] <= 9)
					{
						++digits;
						++index;
					}
					for(cursor = index - 1; cursor > 0; --cursor)
					{
						num2 += data[cursor] * powerr10(index - cursor - 1);
					}
					LCD_displayString("Ans");
					LCD_moveCursor(0, 4);
					LCD_displayCharacter(data[0]);
					LCD_moveCursor(0, 6);
					LCD_intgerToString(num2);
					LCD_displayStringRowColumn(0, 6 + digits, " is:");
					LCD_moveCursor(1, 0);
					if(data[0] == '-')
					{
						ans = ans - num2;
						LCD_intgerToString(ans);
					}
					else if(data[0] == '%')
					{
						ans = ans / num2;
						LCD_intgerToString(ans);
					}
					else if(data[0] == '+')
					{
						ans = ans + num2;
						LCD_intgerToString(ans);
					}
					else if(data[0] == '*')
					{
						ans = ans * num2;
						LCD_intgerToString(ans);
					}
				}
				else
				{
					while(0 <= data[index] && data[index] <= 9)
					{
						++index;
					}
					for(cursor = index - 1; cursor >= 0; --cursor)
					{
						num1 += data[cursor] * powerr10(index - cursor - 1);
					}
					uint8 calc = index;
					index++;
					while(0 <= data[index] && data[index] <= 9)
					{
						++index;
					}
					for(cursor = index - 1; cursor > calc; --cursor)
					{
						num2 += data[cursor] * powerr10(index - cursor - 1);
					}
					if(data[calc] == '-')
					{
						ans = num1 - num2;
						LCD_intgerToString(ans);
					}
					else if(data[calc] == '%')
					{
						ans = num1 / num2;
						LCD_intgerToString(ans);
					}
					else if(data[calc] == '+')
					{
						ans = num1 + num2;
						LCD_intgerToString(ans);
					}
					else if(data[calc] == '*')
					{
						ans = num1 * num2;
						LCD_intgerToString(ans);
					}
				}
			}
			else
			{
				data[cursor] = key;
				LCD_moveCursor(row, col);
				_delay_ms(10);
				if(0 <= data[cursor] && data[cursor] <= 9)
				{
					LCD_intgerToString(data[cursor]);
				}
				else
				{
					LCD_displayCharacter(data[cursor]);
				}
			}
		}
	}
	return 0;
}
