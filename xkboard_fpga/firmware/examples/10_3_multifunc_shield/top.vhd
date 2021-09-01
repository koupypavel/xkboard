library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity multishield_driver is
    Port ( i_clock	: in  std_logic;
			clock	: out std_logic;
			data	: out std_logic;
			st		: out std_logic;
				
			i_switch_s1	: in std_logic;
			i_switch_s2	: in std_logic;
			i_switch_s3	: in std_logic;
			
			o_led_d10 : out std_logic;
			o_led_d11 : out std_logic;
			o_led_d12 : out std_logic;
			o_led_d13 : out std_logic
			);
			
end multishield_driver;

architecture Behavioral of multishield_driver is

constant char_A :std_logic_vector(0 to 7) := "10001000";
constant char_H :std_logic_vector(0 to 7) := "10001001";
constant char_O :std_logic_vector(0 to 7) := "11000000";
constant char_J :std_logic_vector(0 to 7) := "11100001";
constant led_zero :std_logic_vector(0 to 3) := "0000";

constant addr_seg_1 :std_logic_vector(0 to 7) := "00000001";
constant addr_seg_2 :std_logic_vector(0 to 7) := "00000010";
constant addr_seg_3 :std_logic_vector(0 to 7) := "00000100";
constant addr_seg_4 :std_logic_vector(0 to 7) := "00001000";


--constant c_CNT_100KHZ : natural := 60;
--signal r_CNT_100KHZ : natural range 0 to c_CNT_100KHZ;
signal r_CNT_CHAR : natural range 0 to 3;

--stavy pro odeslani 2 bajtu dat
type state is (start, data_0, data_1, data_2, data_3, data_4, data_5, data_6, data_7, addr_0, addr_1, addr_2, addr_3, addr_4, addr_5, addr_6, addr_7, between_delay, delay, write_bit);

signal w_disp_select :std_logic_vector(0 to 7);
signal w_char_select :std_logic_vector(0 to 7);
signal w_leds_state :std_logic_vector(0 to 3) := led_zero;
signal LED_STATE :std_logic_vector(0 to 3) := led_zero;
--signal CLOCK_OUT : STD_LOGIC := '0';
signal EN_OUT : std_logic := '1';
signal DATA_OUT: std_logic := '0';
signal CURR_STATE: state := start;
signal NEXT_STATE: state := start;


begin
--binary LED counter process
led_counter :process(i_clock)
begin
	if rising_edge(i_clock) then
		if(i_switch_s1 = '0') then
			LED_STATE <= std_logic_vector(unsigned(LED_STATE) + 1); 
		else
			if(i_switch_s2 = '0') then
				LED_STATE <= std_logic_vector(unsigned(LED_STATE) - 1); 
			end if;
		end if;
		if(i_switch_s3 = '0') then
			LED_STATE <= led_zero;
		end if;			
	end if;
end process;

--sending 2 bytes to LED 7-segment display 
send_char :process(i_clock, CURR_STATE)
begin
	if rising_edge(i_clock) then
	case CURR_STATE is
			when write_bit =>
			clock <= '0';
			CURR_STATE <= NEXT_STATE;
		when start =>
			CURR_STATE <= data_0;
			EN_OUT<= '0';
			clock <= '0';
		when data_0 =>
			CURR_STATE <= write_bit;
			DATA_OUT <= std_logic(w_char_select(0));
			NEXT_STATE <= data_1;
			clock <= '1';
		when data_1 =>
			CURR_STATE <= write_bit;
			NEXT_STATE <=data_2;
			DATA_OUT <= std_logic(w_char_select(1));
			clock <= '1';
		when data_2 =>
			CURR_STATE <= write_bit;
			NEXT_STATE <= data_3;
			DATA_OUT <= std_logic(w_char_select(2));
			clock <= '1';
		when data_3 =>
			CURR_STATE <= write_bit;
			NEXT_STATE <= data_4;
			DATA_OUT <= std_logic(w_char_select(3));
			clock <= '1';
		when data_4 =>
			CURR_STATE <= write_bit;
			NEXT_STATE <=data_5;
			DATA_OUT <= std_logic(w_char_select(4));
			clock <= '1';
		when data_5 =>
			CURR_STATE <= write_bit;
			NEXT_STATE <= data_6;
			DATA_OUT <= std_logic(w_char_select(5));
			clock <= '1';
		when data_6 =>
			CURR_STATE <= write_bit;
			NEXT_STATE <= data_7;
			DATA_OUT <= std_logic(w_char_select(6));
			clock <= '1';
		when data_7 =>
			CURR_STATE <= write_bit;
			NEXT_STATE <= between_delay;
			DATA_OUT <= std_logic(w_char_select(7));
			clock <= '1';
		when between_delay =>
			CURR_STATE <= addr_0;
		when addr_0 =>
			CURR_STATE <= write_bit;
			NEXT_STATE <= addr_1;
			DATA_OUT <= std_logic(w_disp_select(0));
			clock <= '1';
		when addr_1 =>
			CURR_STATE <= write_bit;
			NEXT_STATE <= addr_2;
			DATA_OUT <= std_logic(w_disp_select(1));
			clock <= '1';
		when addr_2 =>
			CURR_STATE <= write_bit;
			NEXT_STATE <= addr_3;
			DATA_OUT <= std_logic(w_disp_select(2));
			clock <= '1';
		when addr_3 =>
			CURR_STATE <= write_bit;
			NEXT_STATE <= addr_4;
			DATA_OUT <= std_logic(w_disp_select(3));
			clock <= '1';
		when addr_4 =>
			CURR_STATE <= write_bit;
			NEXT_STATE <= addr_5;
			DATA_OUT <= std_logic(w_disp_select(4));
			clock <= '1';
		when addr_5 =>
			CURR_STATE <= write_bit;
			NEXT_STATE <= addr_6;
			DATA_OUT <= std_logic(w_disp_select(5));
			clock <= '1';
		when addr_6 =>
			CURR_STATE <= write_bit;
			NEXT_STATE <= addr_7;
			DATA_OUT <= std_logic(w_disp_select(6));
			clock <= '1';
		when addr_7 =>
			CURR_STATE <= write_bit;
			NEXT_STATE <= delay;
			DATA_OUT <= std_logic(w_disp_select(7));
			clock <= '1';
		when delay =>
			EN_OUT <= '1';
			CURR_STATE <= start;
			if r_CNT_CHAR = 3 then
				r_CNT_CHAR <= 0;
			else
				r_CNT_CHAR <= r_CNT_CHAR + 1;
			end if;
			r_CNT_CHAR<= r_CNT_CHAR + 1;		
		when others =>
			CURR_STATE <= start;
		end case;
	
	end if;
end process;

--multiplex for char
 w_char_select <= char_A when (r_CNT_CHAR = 0) else
                  char_H  when (r_CNT_CHAR = 1) else
                  char_O  when (r_CNT_CHAR = 2) else
                  char_J;
--multiplex for addr
 w_disp_select <= addr_seg_1 when (r_CNT_CHAR = 0) else
                  addr_seg_2  when (r_CNT_CHAR = 1) else
                  addr_seg_3  when (r_CNT_CHAR = 2) else
                  addr_seg_4;
				  
data <= DATA_OUT;	
st <= EN_OUT;
o_led_d10 <= LED_STATE(0);
o_led_d11 <=LED_STATE(1);
o_led_d12 <= LED_STATE(2);
o_led_d13 <=LED_STATE(3);
end Behavioral;