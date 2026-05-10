library ieee;
use ieee.std_logic_1164.all;

entity NOR_AND is 
port (
    a, b: in std_logic;
	q: out std_logic
);
end NOR_AND;

architecture rtl of NOR_AND is
begin
	q <= (a and b) or (a nor b);
end rtl;