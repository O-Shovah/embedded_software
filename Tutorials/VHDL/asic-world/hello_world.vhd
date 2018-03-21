-- https://martin-jones.com/2015/08/26/first-steps-with-a-lattice-ice40-fpga/

LIBRARY ieee;
USE ieee.std_logic_1164.ALL;

ENTITY test IS
 PORT
 (
 d: in std_logic;
 q: out std_logic;
 qn: out std_logic
 );
END test;

ARCHITECTURE rtl OF test IS
BEGIN

 q <= d;
 qn <= not d;
 
END rtl;
