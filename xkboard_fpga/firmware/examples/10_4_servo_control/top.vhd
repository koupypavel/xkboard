library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity arm_control is
	PORT(
		st_i : IN  STD_LOGIC;
		data_i : IN  STD_LOGIC;
		clock_i : IN  STD_LOGIC;
		clk  : IN  STD_LOGIC;
		servo_a: OUT STD_LOGIC;
		servo_b: OUT STD_LOGIC;
		servo_c: OUT STD_LOGIC;
		
		o_led_d1 : out std_logic;
		o_led_d2 : out std_logic;
		o_led_d3 : out std_logic
	);
end arm_control;

architecture Behavioral of arm_control is

	signal clk_out : STD_LOGIC ;
	signal f_pmw_pos_a : STD_LOGIC_VECTOR(4 downto 0):="00110";
	signal f_pmw_pos_b : STD_LOGIC_VECTOR(4 downto 0):="00110";
	signal f_pmw_pos_c : STD_LOGIC_VECTOR(4 downto 0):="00111";
	
    signal temporal: STD_LOGIC;
    signal counter : integer range 0 to 780 := 0;
	
	signal cnt_a : unsigned(10 downto 0);
	signal pwmi_a: unsigned(7 downto 0);
	signal cnt_b : unsigned(10 downto 0);
	signal pwmi_b: unsigned(7 downto 0);
	signal cnt_c : unsigned(10 downto 0);
	signal pwmi_c: unsigned(7 downto 0);
	
	-- states of msg recieving
	type state is (idle, data_0, data_1, data_2, data_3, data_4, data_5, data_6, data_7, parse_cmd);
	
	signal CURR_STATE: state := idle;
	signal CMD : STD_LOGIC_VECTOR(7 downto 0);
	
	constant addr_a :std_logic_vector(0 to 2) := "001";
	constant addr_b :std_logic_vector(0 to 2) := "010";
	constant addr_c :std_logic_vector(0 to 2) := "011";
begin
--clock divider 64 khz
    freq_divider: process (clk) begin
        if rising_edge(clk) then
            if (counter = 187) then
                temporal <= NOT(temporal);
                counter  <= 0;
            else
                counter <= counter + 1;
            end if;
        end if;
    end process;
	
--pwm signal for servo_a
	pwmi_a <= unsigned("000" & f_pmw_pos_a) + 32;
	pwm_gen_a: process (temporal) begin
		if rising_edge(temporal) then
			if (cnt_a = 1279) then
				cnt_a <= (others => '0');
			else
				cnt_a <= cnt_a + 1;
			end if;
		end if;
	end process;
	servo_a <= '1' when (cnt_a < pwmi_a) else '0';

--pwm signal for servo_b
	pwmi_b <= unsigned("000" & f_pmw_pos_b) + 32;
	pwm_gen_b: process (temporal) begin
		if rising_edge(temporal) then
			if (cnt_b = 1279) then
				cnt_b <= (others => '0');
			else
				cnt_b <= cnt_b + 1;
			end if;
		end if;
	end process;
	servo_b <= '1' when (cnt_b < pwmi_b) else '0';
	
--pwm signal for servo_c
	pwmi_c <= unsigned("000" & f_pmw_pos_c) + 32;
	pwm_gen_c: process (temporal) begin
		if rising_edge(temporal) then
			if (cnt_c = 1279) then
				cnt_c <= (others => '0');
			else
				cnt_c <= cnt_c + 1;
			end if;
		end if;
	end process;
	servo_c <= '1' when (cnt_c < pwmi_c) else '0';

--recieving msg from MCU or somthing else 
recv_cmd :process(st_i, clock_i,CURR_STATE)
begin
	if falling_edge(clock_i) then
	case CURR_STATE is
		when idle =>
			o_led_d1 <= '0';
			o_led_d2 <= '0';
			o_led_d3 <= '0';
			CMD <= (others => '0');
			if(st_i= '0') then
				CURR_STATE <= data_0;
			else
				CURR_STATE <= idle;
			end if;
		when data_0 =>
					CURR_STATE <= data_1;
					CMD(7) <= data_i;
		when data_1 =>
					CURR_STATE <= data_2;
					CMD(6) <= data_i;
		when data_2 =>
					CURR_STATE <= data_3;
					CMD(5) <= data_i;
		when data_3 =>
					CURR_STATE <= data_4;
					CMD(4) <= data_i;
		when data_4 =>
					CURR_STATE <= data_5;
					CMD(3) <= data_i;
		when data_5 =>
					CURR_STATE <= data_6;
					CMD(2) <= data_i;
		when data_6 =>
					CURR_STATE <= data_7;
					CMD(1) <= data_i;
		when data_7 =>
					CURR_STATE <= parse_cmd;
					CMD(0) <= data_i;
		when parse_cmd =>
					CURR_STATE <= idle;				
					if(CMD(7) = '0') and (CMD(6) = '0') and (CMD(5) = '1') then
						f_pmw_pos_a <= CMD (4 DOWNTO 0);
						o_led_d1 <= '1';
					end if;
					if(CMD(7) = '0') and (CMD(6) = '1') and (CMD(5) = '0') then
						f_pmw_pos_b <= CMD (4 DOWNTO 0);
						o_led_d2 <= '1';
					end if;
					if(CMD(7) = '0') and (CMD(6) = '1') and (CMD(5) = '1') then
						f_pmw_pos_c <= CMD (4 DOWNTO 0);
						o_led_d3 <= '1';
						
					end if;
		when others =>
			CURR_STATE <= idle;
		end case;
	end if;
end process;
end Behavioral;