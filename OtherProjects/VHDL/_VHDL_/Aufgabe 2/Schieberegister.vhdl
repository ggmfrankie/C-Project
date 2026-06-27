library ieee;
use ieee.std_logic_1164.all;

entity Schieberegister is 
port (
    clk: in std_logic;
    d: in std_logic;
	q: out std_logic_vector (7 downto 0)
);
end Schieberegister;

architecture rtl of Schieberegister is
    signal q_int : std_logic_vector(7 downto 0) := "00000000";
    
begin

	process (clk)
        
    begin
        if rising_edge(clk) then
            q_int(0) <= d;
            for i in 1 to 7 loop
                q_int(i) <= q_int(i-1);
            end loop;
        end if;
    end process;

    q <= q_int;
end rtl;