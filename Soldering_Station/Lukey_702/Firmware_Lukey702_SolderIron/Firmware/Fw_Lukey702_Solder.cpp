/*
* Fw_Lukey702_Solder.cpp
* Lukey702 solder iron control MCU firmware
* Created: 09.01.2019
* Author: vdd-pro.ru
*/

#include "Timeouts.h"
#include "Common.h"


//static uchar flags;

// port C
#define TEMP_ADC_PC		0

// port B
#define A3_PB			0
#define HEATER_PB		1
#define BTN_DOWN_PB		2
#define PRESENT_PB		4
#define BTN_UP_PB		5
#define A1_PB			6
#define A2_PB			7

// port D
#define LED_A_PD		0
#define LED_E_PD		1
#define LED_D_PD		2
#define LED_F_PD		3
#define LED_DP_PD		4
#define LED_C_PD		5
#define LED_G_PD		6
#define LED_B_PD		7



void inline TimersInit(){
	// TCNT1 is for a real time clock
	TCCR1A = 0;
	// Normal mode, clk/1024 = 7812.5 Hz (for 8 MHz)
	// 65535 -> ~8.3 sec
	TCCR1B = (1 << CS10) | (0 << CS11) | (1 << CS12);
	// disable timer interrupts
	TIMSK1 = 0;
}

void inline ADC_SelectChannel(uchar ChN){
	ADMUX = (0<<REFS0)|(0<<REFS1)|(0<<ADLAR) | ChN;// select ChN pin as input with external Vref (2.5V)
	ADCSRA |= (1<<ADIF)|(1<<ADSC); // start conversion
}

void inline ADC_Init(void){ // needed for reference configuration
	ADCSRA = (1<<ADEN)|(0<<ADATE)|(1<<ADPS2)|(1<<ADPS1)|(0<<ADPS0); // not-autotrigger mode with clock / 64
	ADCSRB = 0; // free running mode
}


/* ------------------------------------------------------------------------- */

void WDT_off(void){
	cli();
	wdt_reset();
	/* Clear WDRF in MCUSR */
	MCUSR &= ~(1<<WDRF);
	/* Write logical one to WDCE and WDE */
	/* Keep old prescaler setting to prevent unintentional time-out */
	WDTCSR |= (1<<WDCE) | (1<<WDE);
	/* Turn off WDT */
	WDTCSR = 0x00;
	//__enable_interrupt();
}

void WDT_on(void){
	wdt_reset();
	/* Set WDRF in MCUSR */
	MCUSR |= (1<<WDRF);
	/* Start timed sequence */
	WDTCSR |= (1<<WDCE) | (1<<WDE);
	/* Set new prescaler(time-out) value = 1024K (1048576) cycles (8.0 s) */
	WDTCSR = (1<<WDE) | (1<<WDP3) | (1<<WDP0);
	//__enable_interrupt();
}

// LED display controller
class cDisplay{
	char Digit[3]; // digits as ASCI characters
	cTimeoutMs ShowTim; // Digit's change timer
	cTimeoutSec disp_mode_to; // timeout for switch back from the setup temperature to the current one displaying
	bool cur_mode_set;
	uchar pos; // current digit
	
	public:
	uchar Dp[3]; // active dots
	
	cDisplay():ShowTim(3), disp_mode_to(3), cur_mode_set(false){
		Dp[0] = Dp[1] = Dp[2] = 0;
	}
	
	// set the number to display
	void NumToDigits(uint num){
		if(num > 999) num = 999;
		uchar n100 = num / 100;
		Digit[0] = n100 ? n100 + '0' : ' ';
		if(n100) num -= n100 * 100;
		
		uchar n10 = num / 10;
		Digit[1] = n10 || n100 ? n10 + '0' : ' ';
		if(n10) num -= n10 * 10;
		
		Digit[2] = num + '0';
	}
	
	void ShowDashes(){
		if(cur_mode_set){
			cur_mode_set = !disp_mode_to();
			return;
		}
		Digit[0] = Digit[1] = Digit[2] = '-';
	}
	
	void ShowCurTemp(uint temp){
		if(cur_mode_set){
			cur_mode_set = !disp_mode_to();
			return;
		}
		NumToDigits(temp);
	}
	
	void ShowSetTemp(uint temp){
		disp_mode_to.Reset();
		cur_mode_set = true;
		NumToDigits(temp);
	}
	
	
	void Handler(){
		if(!ShowTim()) return;
		ShowChar();
		pos++;
	}
	
	private:
	// our charmap - converts ASCII char to a LED segments mask
	uchar CharToMask(char ch){
		switch (ch){
			case '0':
			return 0x3F;
			case '1':
			return 0x06;
			case '2':
			return 0x5B;
			case '3':
			return 0x4F;
			case '4':
			return 0x66;
			case '5':
			return 0x6D;
			case '6':
			return 0x7D;
			case '7':
			return 0x07;
			case '8':
			return 0x7F;
			case '9':
			return 0x6F;
			case '-':
			return 0x40;
			case 'h':
			return 0x74;
			case 't':
			return 0x78;
		}
		return 0;
	}
	
	// pos must be 0, 1, 2
	void ShowChar(){
		if (pos >= 3)
		pos = 0;

		uchar bits = CharToMask(Digit[pos]);

		// turn the light OFF
		PORTB &= ~((1 << A1_PB) | (1 << A2_PB) | ( 1 << A3_PB));
		
		// parse digits from mask

		for (uchar i = 0; i < 7; i++){
			uchar state = ((bits >> i) & 0x01) ? 0 : 1;
			switch (i){
				case 0:
				WritePortBit(PORTD, LED_A_PD, state);
				break;
				case 1:
				WritePortBit(PORTD, LED_B_PD, state);
				break;
				case 2:
				WritePortBit(PORTD, LED_C_PD, state);
				break;
				case 3:
				WritePortBit(PORTD, LED_D_PD, state);
				break;
				case 4:
				WritePortBit(PORTD, LED_E_PD, state);
				break;
				case 5:
				WritePortBit(PORTD, LED_F_PD, state);
				break;
				case 6:
				WritePortBit(PORTD, LED_G_PD, state);
				break;
			}
		}
		// show dot
		uchar state = Dp[pos] ? 0 : 1;
		WritePortBit(PORTD, LED_DP_PD, state);
		// turn on only one digit
		switch (pos){
			case 0:
			WritePortBit(PORTB, A1_PB, 1);
			break;
			case 1:
			WritePortBit(PORTB, A2_PB, 1);
			break;
			case 2:
			WritePortBit(PORTB, A3_PB, 1);
			break;
		}
	}
	
} Display;

// ADC converter shell
class cAdc{
	uint val_Temp; // raw values (12-bit)
	public:
	uint Tempx10; // degrees C x10 to get better resolution for the regulator
	
	cAdc(): val_Temp(0)
	{
	}
	
	void Handler(){
		//check for the ADC conversion finish
		if((ADCSRA & (1<<ADSC)) != 0) // conversion is in progress
		return;
		
		uchar cur_ch = ADMUX & 0x0F;
		switch(cur_ch){
			case TEMP_ADC_PC:
			AdcAvg(val_Temp);
			ADC_SelectChannel(TEMP_ADC_PC);
			// convert to degrees
			// solder iron thermocouple signal conversion approximation
			// T[C] = U[V] * k + 20  
			// k = 263 if T < 380; else k = 280
			Tempx10 = (uint32_t(val_Temp) * 2500 / 4095) * // converted to mV; reference voltage is 2500 mV
			(Tempx10 < 3800 ? 263 : 280) / 100 + 200; // convert to *C x10
			break;
			
			default:
			ADC_SelectChannel(TEMP_ADC_PC);
		}
	}
	
	private:
	
	void AdcAvg(uint &arg){
		// average ADC value (10 bit) and convert to 12 bit
		arg = ((ADC << 2) + arg * 3) >> 2;
	}
	
} Adc;

class cRegulator{
	cTimeoutMs reg_to;
	
	public:
	uint Temp; // desired temperature, *C
	
	cRegulator(): reg_to(100), Temp(200){
		
	}
	
	void TempUp(){
		if(Temp < 480)
		Temp ++;
	}
	
	void TempDown(){
		if(Temp > 200)
		Temp --;
	}
	
	void Handler(){
		if(!reg_to())
		return;
		bool bConnected = IsConnected();
		bool bHeaterOn = bConnected && Temp * 10 > Adc.Tempx10;
		if(bConnected)
		{ // make current temperature display more nice
			uint curtemp = (Adc.Tempx10 + 5) / 10;
			if(curtemp > Temp - 5 && curtemp < Temp + 10) curtemp = Temp;
			Display.ShowCurTemp(curtemp);
		}else
		{
			Display.ShowDashes(); // Solder iron is missing
		}
		
		HeaterControl(bHeaterOn);
	}
	
	bool IsConnected(){
		return (PINB & (1 << PRESENT_PB)) == 0; // active level is low
	}
	
	private:
	void HeaterControl(bool bTurnOn){
		WritePortBit(PORTB, HEATER_PB, !bTurnOn);
		Display.Dp[2] = bTurnOn; // show dot when the heater is ON
	}
}Regulator;


class cButtons{
	cTimeoutMs handle_to;
	cTimeoutMs update_to;
	uint round_cnt;
	public:
	
	cButtons():handle_to(20), update_to(500){}
	
	void Handler(){
		if(!handle_to()) return;
		
		bool bUp = (PINB & (1 << BTN_UP_PB)) == 0; // active level is low
		bool bDown = (PINB & (1 << BTN_DOWN_PB)) == 0;
		
		if(!bUp && !bDown){
			round_cnt = 0;
			return;
		}
		
		if(!update_to() && round_cnt > 0) return;
		 
		if(bUp) Regulator.TempUp();
		
		if(bDown) Regulator.TempDown();
		
		// speed-up increment or decrement while button holding
		round_cnt ++;
		uint period_ms = 500 / round_cnt;
		if(period_ms < 50) period_ms = 50;
		update_to.SetPeriod(period_ms);
		
		Display.ShowSetTemp(Regulator.Temp);
		
	}
}Buttons;

// =====================================================================

// EEPROM START
EEMEM uint eeTemp, eeMagic;
void inline LoadConfig(){
	uint magic = eeprom_read_word(&eeMagic);
	if(magic != 0xABBA) return; // no saved config - use default values
	Regulator.Temp =  eeprom_read_word(&eeTemp);
}

void inline SaveConfig(){
	eeprom_update_word(&eeMagic, 0xABBA);
	eeprom_update_word(&eeTemp, Regulator.Temp);
}
// EEPROM END


int main(void){
	
	WDT_off();
	WDT_on();   // apply WDT timeout
	cli();
	
	// ports initial configuration
	DDRC = 0; // all inputs
	PORTC = 0xFE; //all pull-up except PC0 (ADC0)
	
	DDRB = (1 << A1_PB) | (1 << A2_PB) | (1 << A3_PB) | (1 << HEATER_PB) | (0 << BTN_DOWN_PB) | (0 << BTN_UP_PB) | (0 << PRESENT_PB);
	PORTB = (1 << BTN_DOWN_PB) | (1 << BTN_UP_PB) | (1 << PRESENT_PB); // pull-up inputs
	
	DDRD = 0xFF; // all outputs to the LED display
	PORTD = 0; // turn on all digits
	
	// load config from EEPROM
	LoadConfig();
	
	// prepare timers
	TimersInit();
	ADC_Init();
	
	cTimeoutMs OneSec(1000);
	cTimeoutSec eep_save_to(20); // update current temperature settings in EEPROM with 20 seconds interval

	while(1){
		Display.Handler();
		Adc.Handler();
		Regulator.Handler();
		Buttons.Handler();
		
		if(eep_save_to())
		{
			SaveConfig();
		}
		
		if(OneSec()){ // 1 second timeout
			Seconds ++;
			//Display.NumToDigits(Seconds);
		}
		
		wdt_reset();
	}
}

/* ------------------------------------------------------------------------- */

