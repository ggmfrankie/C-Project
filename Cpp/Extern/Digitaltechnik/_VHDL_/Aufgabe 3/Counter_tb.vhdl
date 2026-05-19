library ieee;
use ieee.std_logic_1164.all;
entity Counter_tb is end;
architecture sim of Counter_tb is
    signal clk : std_logic := '0';
    signal nClear : std_logic := '0';
    signal nLoad : std_logic := '0';
    signal d_intput : std_logic_vector(3 downto 0) := (others => '0');
    signal p : std_logic := '0';
    signal t : std_logic := '0';
    signal q1 : std_logic := '0';
    signal q2 : std_logic := '0';
    signal q3 : std_logic := '0';
    signal q4 : std_logic := '0';
    signal carry : std_logic := '0';
begin
    port_inst: entity work.Counter
         port map (
            clk => clk,
            nClear => nClear,
            nLoad => nLoad,
            d_intput => d_intput,
            p => p,
            t => t,
            q1 => q1,
            q2 => q2,
            q3 => q3,
            q4 => q4,
            carry => carry
         );
    clk_proc: process
    begin
        for i in 0 to 30 loop
            clk <= '0'; wait for 125.0 ns;
            clk <= '1'; wait for 125.0 ns;
        end loop;
        wait;
    end process;

    tb: process
    begin
            p <= '1'; t <= '1'; nLoad <= '1'; nClear <= '1'; wait for 6000 ns;
            p <= '0'; t <= '0'; nLoad <= '0'; d_intput <= "1010"; wait for 2000 ns;
            p <= '0'; t <= '0'; nLoad <= '1'; nClear <= '0'; d_intput <= "1010"; wait for 2000 ns;

        wait;
    end process;
end architecture;