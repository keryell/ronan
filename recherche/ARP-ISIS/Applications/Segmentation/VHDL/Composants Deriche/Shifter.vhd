-- =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- --
-- Shifter for CPM Fifo Output Interface                        --
-- Transmission Core (PipeCore)                                 --
--                                                              --
-- Author:     Serge Savard                                     --
-- $State: $                                                    --
-- $Revision: $                                                 --
-- $Date: $                                                     --
--                                                              --
-- $Log: $                                                      --
--                                                              --
-- (c) Copyright MiroTech Microsystems 1995-1996                --
-- =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- --

library ieee;
use ieee.std_logic_1164.all;

entity shifter is
	port (
		--=--- Module Interface --=--
		clk		: in std_logic;
		frame_in	: in std_logic_vector(7 downto 0);
		load_en		: in std_logic;
		shift_en	: in std_logic;
		--=-- Data out --=--
		data_out	: out std_logic_vector(7 downto 0)
	);

end shifter;

architecture behavioral of shifter is

	-- -=-=-=-=-=-=-=-=-=- --
	-- Signal Declarations --
	-- -=-=-=-=-=-=-=-=-=- --

	signal frame_buf : std_logic_vector (31 downto 0);

begin
	-- ----------------------------- --
	-- ++ -- Basic Assignments -- ++ -- 

	data_out <= frame_buf(7 downto 0);


	-- ----------------------------- --
	-- ++ -- Fifos Description -- ++ -- 

	FrameBuff : process(clk)
	begin
		if rising_edge(clk) then
			if (load_en = '1') then
				frame_buf(31 downto 24) <= frame_in;
			elsif (shift_en = '1') then
				frame_buf(23 downto 0) <= frame_buf(31 downto 8);
			end if;
		end if;
	end process; 
end behavioral;
