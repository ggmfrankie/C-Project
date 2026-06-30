library ieee;
use ieee.std_logic_1164.all;

entity JK_FlipFlop is 
port (
    j, k, clk: in std_logic;
	q, nq: out std_logic
);
end JK_FlipFlop;

architecture rtl of JK_FlipFlop is
    signal jk : std_logic_vector(1 downto 0);
    signal q_int : std_logic := '0';
begin
    jk <= j & k;
	process (clk)
        
    begin
        if rising_edge(clk) then
            case (jk) is
                when "00" =>
                    q_int <= q_int;

                when "01" =>
                    q_int <= '0';

                when "10" =>
                    q_int <= '1';

                when "11" =>
                    q_int <= not q_int;

                when others =>
                    q_int <= '0';
            end case;
        end if;
    end process;
    q <= q_int;
    nq <= not q_int;
end rtl;