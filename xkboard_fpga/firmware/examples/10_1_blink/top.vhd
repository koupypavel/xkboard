library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity tutorial_led_blink is
  port (
    i_clock      : in  std_logic;
    i_switch_s1   : in  std_logic;
    i_switch_s2   : in  std_logic;
    o_led_d1 : out std_logic;
	o_led_d2 : out std_logic;
	o_led_d3 : out std_logic
    );
end tutorial_led_blink;
 
architecture rtl of tutorial_led_blink is
 
  -- citace pro jednotlive frekvence
  -- 12,000,000 Hz (MCU core freq) / 20 Hz (desired freq) * 0.5 (duty cycle) = 300 000
  constant c_CNT_20HZ : natural := 300000;
  constant c_CNT_10HZ  : natural := 600000;
  constant c_CNT_5HZ  : natural := 1200000; 
  constant c_CNT_1HZ   : natural := 6000000;
 
 
  -- These signals will be the counters:
  signal r_CNT_20HZ : natural range 0 to c_CNT_20HZ;
  signal r_CNT_10HZ  : natural range 0 to c_CNT_10HZ;
  signal r_CNT_5HZ  : natural range 0 to c_CNT_5HZ;
  signal r_CNT_1HZ   : natural range 0 to c_CNT_1HZ;
   
  -- These signals will toggle at the frequencies needed:
  signal r_TOGGLE_20HZ : std_logic := '0';
  signal r_TOGGLE_10HZ  : std_logic := '0';
  signal r_TOGGLE_5HZ  : std_logic := '0';
  signal r_TOGGLE_1HZ   : std_logic := '0';
 
  -- One bit select wire.
  signal w_LED_SELECT : std_logic;
   
begin
 
  -- Procesy citaji svoje frekvence paralelnì a jen pomocí tlacitek
  -- volime, ktery signal bude 
  p_5_HZ : process (i_clock) is
  begin
    if rising_edge(i_clock) then
      if r_CNT_5HZ = c_CNT_5HZ-1 then  -- -1, since counter starts at 0
        r_TOGGLE_5HZ <= not r_TOGGLE_5HZ;
        r_CNT_5HZ    <= 0;
      else
        r_CNT_5HZ <= r_CNT_5HZ + 1;
      end if;
    end if;
  end process p_5_HZ;
 
 
  p_20_HZ : process (i_clock) is
  begin
    if rising_edge(i_clock) then
      if r_CNT_20HZ = c_CNT_20HZ-1 then
        r_TOGGLE_20HZ <= not r_TOGGLE_20HZ;
        r_CNT_20HZ    <= 0;
      else
        r_CNT_20HZ <= r_CNT_20HZ + 1;
      end if;
    end if;
  end process p_20_HZ;
 
   
  p_10_HZ : process (i_clock) is
  begin
    if rising_edge(i_clock) then
      if r_CNT_10HZ = c_CNT_10HZ-1 then
        r_TOGGLE_10HZ <= not r_TOGGLE_10HZ;
        r_CNT_10HZ    <= 0;
      else
        r_CNT_10HZ <= r_CNT_10HZ + 1;
      end if;
    end if;
  end process p_10_HZ;
 
   
  p_1_HZ : process (i_clock) is
  begin
    if rising_edge(i_clock) then
      if r_CNT_1HZ = c_CNT_1HZ-1 then 
        r_TOGGLE_1HZ <= not r_TOGGLE_1HZ;
        r_CNT_1HZ    <= 0;
      else
        r_CNT_1HZ <= r_CNT_1HZ + 1;
      end if;
    end if;
  end process p_1_HZ;
 
   
  -- multiplexor pro prepinace a volbu signalu
  w_LED_SELECT <= r_TOGGLE_1HZ when (i_switch_s1 = '0' and i_switch_s2 = '0') else
                  r_TOGGLE_5HZ  when (i_switch_s1 = '0' and i_switch_s2 = '1') else
                  r_TOGGLE_10HZ  when (i_switch_s1 = '1' and i_switch_s2 = '0') else
                  r_TOGGLE_20HZ;
 
   
  -- prirazeni vybraneho singnalu vsem ledkam
  o_led_d1 <= w_LED_SELECT;
  o_led_d2 <= w_LED_SELECT;
  o_led_d3 <= w_LED_SELECT;
 
end rtl;