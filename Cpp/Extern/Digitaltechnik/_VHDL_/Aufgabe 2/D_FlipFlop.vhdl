library ieee;
use ieee.std_logic_1164.all;

entity D_FlipFlop is 
port (
    a, clk: in std_logic;
	q: out std_logic
);
end D_FlipFlop;

architecture rtl of D_FlipFlop is
begin
	process (clk)
    begin
        if rising_edge(clk) then
            q <= a;
        end if;
    end process;
end rtl;