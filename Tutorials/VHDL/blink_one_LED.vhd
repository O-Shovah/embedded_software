 library IEEE;
 use IEEE.STD_LOGIC_1164.ALL;
 use IEEE.numeric_std.all;
 
 entity led is
    Port ( 
    	refclk 	: in std_logic;
    	rst_in 	: in std_logic;
    	led_0 	: out std_logic;
        led_1 	: out std_logic;
        led_2	: out std_logic;
        led_3	: out std_logic
         
    );
 end led;
 
 architecture RTL of led is
    constant 	max_count	: natural	:= 3330000;
    constant 	timer_0 	: natural 	:= 33300000; 
    signal 		Rst 		: std_logic := '0';
    signal		reset 		: std_logic := '0';

 begin

 	reset <= rst_in;

    -- 0 to max_count counter
    Tescht : process(refclk, reset)
        variable count : natural range 0 to max_count;
    begin

    	
    	if reset = '0' then
            count := 0;
            led_0 <= '1';
            led_1 <= '1';
        elsif rising_edge(refclk) then
            if count < (max_count - 1) / 2 then
                count := count + 1;
                led_0 <= '1';
                led_1 <= '0';
            elsif count < (max_count - 1) then
                led_0 <= '0';
                led_1 <= '1';
                count := count + 1;
            else
                led_0 <= '1';
                led_1 <= '0';
                count := 0;
            end if;
        end if;

    end process Tescht; 


    Reset : process(refclk)
    	variable count : natural range 0 to timer_0;
    begin

    	if rising_edge(refclk) then

    	    if count < (timer_0 - 1) then
                count := count + 1;
            else
            	Rst <= not Rst;
                count := 0;
    		end if;

    	end if;	
	
	end process Reset;

	Input_switch : process(refclk, rst_in)
		
	begin

			if (rst_in = '0') then
				led_2 <= '1';
				led_3 <= '0';
			else
				led_2 <= '0';
				led_3 <= '1';

			end if;

	end process Input_switch;
			
 end RTL;