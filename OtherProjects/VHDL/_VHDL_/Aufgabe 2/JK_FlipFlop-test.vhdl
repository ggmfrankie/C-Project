------------------------------------------------
-- THIS IS A TESTBENCH                        --
-- CREATOR:                                   --
-- OWNER:                                     --
------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;

entity testbench_jk is
end testbench_jk;

architecture rtl of testbench_jk is

    signal clk  : std_logic := '0';
    signal j  : std_logic := '0';
    signal k  : std_logic := '0';
    signal out1 : std_logic;

begin

    port_inst: entity work.JK_FlipFlop
        port map (
            clk => clk,
            j => j,
            k => k,
            q => out1
        );

    clock: process
    begin
        for i in 0 to 8  loop
            clk <= '0';wait for 150 ns;
            clk <= '1';wait for 150 ns;
        end loop;
        wait;
    end process;

    tb: process
    begin
        for i in 1 to 2 loop

            j <= '0'; k <= '1'; wait for 250 ns;
            j <= '0'; k <= '0'; wait for 250 ns;
            j <= '1'; k <= '0'; wait for 250 ns;
            j <= '1'; k <= '1'; wait for 250 ns;

        end loop;

        wait;
    end process tb;

end rtl;