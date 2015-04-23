-- =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- --
-- CPM Fifo Input Interface					--
-- Transmission Core (PipeCore)                                 --
--                                                              --
-- Author:     Serge Savard _ Modifie par N.Viteau              --
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

entity fifo_in is
	port (
		--=-- Module Interface --=--
		Engclk		: in std_logic; 
		Data		: out std_logic_vector(7 downto 0);
		FrameAvail	: out std_logic;
		FrameTaken	: in std_logic; 
		RESET_in	: in std_logic; 	
		--=-- CPM Interface --=--
		FifoEn		: out std_logic;
		FifoRdy		: in std_logic;
		FifoData	: in std_logic_vector(7 downto 0)
	);

end fifo_in;

architecture behavioral of fifo_in is

	-- -=-=-=-=-=-=-=-=-= --
	-- States definitions --
	-- -=-=-=-=-=-=-=-=-= --

	type input_states_def is (PowerUp, WaitByte1, LoadByte2, LoadByte3, LoadByte4, WFT1, WFTB1, WFT2, WFTB2, WFT3, WFTB3, WFT4);

	attribute enum_encoding : string;
	attribute enum_encoding of input_states_def	: type is "one hot";

	signal present_in_state, next_in_state		: input_states_def := PowerUp;

	-- -=-=-=-=-=-=-=-=-=- --
	-- Signals definitions --
	-- -=-=-=-=-=-=-=-=-=- --
	signal Frame_out	: std_logic_vector(31 downto 0);
	signal FrameBuf		: std_logic_vector(31 downto 0);
	signal byte_en		: std_logic_vector(3 downto 0);

	-- -=-=-=-=-=-=-=-=-=-=-= --
	-- Components definitions --
	-- -=-=-=-=-=-=-=-=-=-=-= --

	component Framer
	port (
		clk		: in std_logic;
		data_in		: in std_logic_vector(7 downto 0);
		store_en	: in std_logic;
		byte_en		: in std_logic_vector(3 downto 0);
		frame_out	: out std_logic_vector(31 downto 0)
	);
	end component;
	
begin
	-- ----------------------------- --
	-- ++ -- Basic Assignments -- ++ --

	Frame_out <= FrameBuf;

	-- ------------------------------------- --
	-- ++ -- State Machine definitions -- ++ --

	FSMdef: process(engclk, RESET_in)
	begin
		if (RESET_in = '1') then
			present_in_state <= PowerUp;
		elsif rising_edge(engclk) then
			present_in_state <= next_in_state;
		end if;
	end process;


	-- ---------------------------------- --
	-- ++ -- State machine behavior -- ++ --

	FSM1: process(present_in_state, FifoRdy, FrameTaken)
	begin
		case present_in_state is
			when PowerUp =>
				next_in_state <= WaitByte1;

			when WaitByte1 =>
				if (FifoRdy = '1') then
					next_in_state <= LoadByte2;
				else
					next_in_state <= WaitByte1;
				end if;

			when LoadByte2 =>
				next_in_state <= LoadByte3;

			when LoadByte3 =>
				next_in_state <= LoadByte4;

			when LoadByte4 =>
				next_in_state <= WFT1;

			when WFT1 =>
				if (FrameTaken = '1') then
					next_in_state <= WFTB1;
				else
					next_in_state <= WFT1;
				end if;

			when WFTB1 =>
				if (FrameTaken = '0') then
					next_in_state <= WFT2;
				else
					next_in_state <= WFTB1;
				end if;
				
			when WFT2 =>
				if (FrameTaken = '1') then
					next_in_state <= WFTB2;
				else
					next_in_state <= WFT2;
				end if;

			when WFTB2 =>
				if (FrameTaken = '0') then
					next_in_state <= WFT3;
				else
					next_in_state <= WFTB2;
				end if;

			when WFT3 =>
				if (FrameTaken = '1') then
					next_in_state <= WFTB3;
				else
					next_in_state <= WFT3;
				end if;

			when WFTB3 =>
				if (FrameTaken = '0') then
					next_in_state <= WFT4;
				else
					next_in_state <= WFTB3;
				end if;

			when WFT4 =>
				if (FrameTaken = '1') then
					next_in_state <= WaitByte1;
				else
					next_in_state <= WFT4;
				end if;

		end case;
	end process;


	-- ------------------------------------------- --
	-- ++ -- State Machine Dependant Signals -- ++ --

	byte_en(0) <= '1' when (present_in_state = WaitByte1) else '0';

	byte_en(1) <= '1' when (present_in_state = LoadByte2) else '0';

	byte_en(2) <= '1' when (present_in_state = LoadByte3) else '0';

	byte_en(3) <= '1' when (present_in_state = LoadByte4) else '0';

	FifoEn <= '0' when ((present_in_state = WaitByte1) or
                        (present_in_state = LoadByte2) or
                        (present_in_state = LoadByte3) or
                        (present_in_state = LoadByte4)) else '1';

	FrameAvail <= '1' when (present_in_state = WFT1) or
			(present_in_state = WFT2) or
			(present_in_state = WFT3) or
			(present_in_state = WFT4) else '0';

	with present_in_state select
		Data(7 downto 0) <=
		Frame_out(15 downto 8) when WFTB1,
         	Frame_out(15 downto 8) when WFT2,
         	Frame_out(23 downto 16) when WFTB2,
         	Frame_out(23 downto 16) when WFT3,
         	Frame_out(31 downto 24) when WFTB3,
         	Frame_out(31 downto 24) when WFT4,
         	Frame_out(7 downto 0) when others;

	-- ---------------------------- --
	-- ++ -- ReFraming Buffer -- ++ --

	Frm : FRAMER port map
		(
			clk => engclk,
			data_in => fifoData,
			store_en => '1',
			byte_en => byte_en,
			frame_out => FrameBuf
		);


end behavioral;

