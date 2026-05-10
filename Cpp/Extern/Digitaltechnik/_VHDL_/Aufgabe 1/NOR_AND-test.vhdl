------------------------------------------------
-- THIS IS A TESTBENCH                        --
-- CREATOR:                                   --
-- OWNER:                                     --
------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;

entity testbench is
end testbench;

architecture rtl of testbench is

    signal in1  : std_logic := '0';
    signal in2  : std_logic := '0';
    signal out1 : std_logic;

begin

    port_inst: entity work.NOR_AND
        port map (
            a => in1,
            b => in2,
            q => out1
        );

    tb: process
    begin
        for i in 1 to 2 loop

            in1 <= '0'; in2 <= '0'; wait for 250 ns;
            in1 <= '1'; in2 <= '0'; wait for 250 ns;
            in1 <= '0'; in2 <= '1'; wait for 250 ns;
            in1 <= '1'; in2 <= '1'; wait for 250 ns;

        end loop;

        wait;
    end process tb;

end rtl;