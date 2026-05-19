library ieee;
use ieee.std_logic_1164.all;
use IEEE.NUMERIC_STD.all;

entity Counter is 
port (
    nClear, nLoad : in std_logic;
	d_intput : in std_logic_vector(3 downto 0);
    clk : in std_logic;
    p, t : in std_logic;
    
    q1,q2,q3,q4 : out std_logic;
    carry : out std_logic
);
end Counter;

architecture rtl of Counter is
    signal q_int : std_logic_vector(4 downto 0) := "00000";
begin

    process(clk, nLoad, nClear)
    begin
        if nLoad = '0' then
            q_int(3 downto 0) <= d_intput;
        elsif nClear = '0' then
            q_int <= "00000";
        end if;
        if rising_edge(clk) then
            if (p = '1' and t = '1') then
                q_int <= std_logic_vector(unsigned(q_int) + 1);
            end if;
        end if;
    end process;

    carry <= q_int(4);
    q1 <= q_int(0);
    q2 <= q_int(1);
    q3 <= q_int(2);
    q4 <= q_int(3);
    
end rtl;