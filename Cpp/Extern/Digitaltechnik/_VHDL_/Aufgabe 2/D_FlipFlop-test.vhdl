------------------------------------------------
-- THIS IS A TESTBENCH                        --
-- CREATOR:                                   --
-- OWNER:                                     --
------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;

entity testbench_dff is
end testbench_dff;

architecture rtl of testbench_dff is

    signal clk  : std_logic := '0';
    signal in2 : std_logic := '0';
    signal out1 : std_logic;

begin

    port_inst: entity work.D_FlipFlop
        port map (
            clk => clk,
            a => in2,
            q => out1
        );

    tb: process
    begin
        for i in 1 to 2 loop

            clk <= '0'; in2 <= '0'; wait for 250 ns;
            clk <= '1'; in2 <= '0'; wait for 250 ns;
            clk <= '0'; in2 <= '1'; wait for 250 ns;
            clk <= '1'; in2 <= '1'; wait for 250 ns;

        end loop;

        wait;
    end process tb;

end rtl;