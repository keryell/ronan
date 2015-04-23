-- =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- --
-- Frame for CPM Fifo Input Interface							--
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

entity framer is
	port (

			--=--- Module Interface --=--

			clk		: in std_logic;
			data_in		: in std_logic_vector(7 downto 0);
			store_en	: in std_logic;
			byte_en		: in std_logic_vector(3 downto 0);

			--=-- Frame out --=--

			frame_out	: out std_logic_vector(31 downto 0)
	);

end framer;

architecture behavioral of framer is

	-- -=-=-=-=-=-=-=-=-=- --
	-- Signal Declarations --
	-- -=-=-=-=-=-=-=-=-=- --

	signal frame_buf	: std_logic_vector (31 downto 0);


begin

	-- ----------------------------- --
	-- ++ -- Basic Assignments -- ++ -- 

	frame_out <= frame_buf;


	-- ----------------------------- --
	-- ++ -- Fifos Description -- ++ -- 

	FrameBuff : process(clk)
	begin
		if rising_edge(clk) then
			if (store_en = '1') then
				if (byte_en(0) = '1') then
					Frame_Buf(7 downto 0) <= Data_in;
				end if;

				if (byte_en(1) = '1') then
					Frame_Buf(15 downto 8) <= Data_in;
				end if;

				if (byte_en(2) = '1') then
					Frame_Buf(23 downto 16) <= Data_in;
				end if;

				if (byte_en(3) = '1') then
					Frame_Buf(31 downto 24) <= Data_in;
				end if;
			end if;
		end if;
	end process;


end behavioral;
