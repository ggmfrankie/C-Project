library ieee;
use ieee.std_logic_1164.all;
entity JK_FlipFlop_tb is end;
architecture sim of JK_FlipFlop_tb is
    signal clk : std_logic := '0';
    signal j : std_logic := '0';
    signal k : std_logic := '0';
    signal q : std_logic := '0';
    signal nq : std_logic := '0';
begin
    port_inst: entity work.JK_FlipFlop
         port map (
            clk => clk,
            j => j,
            k => k,
            q => q,
            nq => nq
         );
    clk_proc: process
    begin
        for i in 0 to 20 loop
            clk <= '0'; wait for 125.0 ns;
            clk <= '1'; wait for 125.0 ns;
        end loop;
        wait;
    end process;

    tb: process
    begin
        j <= '0';
        k <= '0';
       wait for 250 ns;
        j <= '0';
        k <= '1';
       wait for 250 ns;
        j <= '1';
        k <= '0';
       wait for 250 ns;
        j <= '1';
        k <= '1';
       wait for 250 ns;
        wait;
    end process;
end architecture;