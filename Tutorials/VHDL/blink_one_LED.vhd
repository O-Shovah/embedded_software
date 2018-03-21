 library IEEE;
 use IEEE.STD_LOGIC_1164.ALL;
 use IEEE.numeric_std.all;
 
 entity led is
    Port ( 
            refclk : in  std_logic;
            led_0 : out std_logic
            led_1 : out std_logic
    );
 end led;
 
 architecture RTL of led is
    constant max_count : natural := 3330000;
    signal Rst : std_logic;
 begin

    Rst <= '0';
 
    -- 0 to max_count counter
    Tescht : process(refclk, Rst)
        variable count : natural range 0 to max_count;
    begin
        if Rst = '1' then
            count := 0;
            led_0 <= '1';
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
 end RTL;