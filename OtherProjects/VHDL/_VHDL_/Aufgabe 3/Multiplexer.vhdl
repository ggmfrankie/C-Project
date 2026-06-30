library ieee;
use ieee.std_logic_1164.all;

entity Multiplexer is 
port (
    sel: in std_logic_vector(1 downto 0);
    a, b, c, d: in std_logic;
	q: out std_logic
);
end Multiplexer;

architecture rtl of Multiplexer is
    signal q_int: std_logic := '0';
begin
    q <= q_int;
	with sel select
        q_int <=    a when "00",
                    b when "01",
                    c when "10",
                    d when others;
end rtl;