------------------------------------------------
-- THIS IS A TESTBENCH                        --
-- CREATOR:                                   --
-- OWNER:                                     --
------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;

entity testbench_sr is
end testbench_sr;

architecture rtl of testbench_sr is

    signal clk  : std_logic := '0';
    signal d  : std_logic := '0';
    signal out1 : std_logic_vector (7 downto 0);

begin

    port_inst: entity work.Schieberegister
        port map (
            clk => clk,
            d => d,
            q => out1
        );

    tb: process
    begin
        
            clk <= '0'; d <= '0'; wait for 250 ns;
            clk <= '1'; d <= '0'; wait for 250 ns;
            clk <= '0'; d <= '1'; wait for 250 ns;
            clk <= '1'; d <= '1'; wait for 250 ns;

            clk <= '0'; d <= '1'; wait for 250 ns;
            clk <= '1'; d <= '0'; wait for 250 ns;
            clk <= '0'; d <= '0'; wait for 250 ns;
            clk <= '1'; d <= '1'; wait for 250 ns;

        wait;
    end process tb;

end rtl;