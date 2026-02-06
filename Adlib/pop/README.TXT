=============================================================================

                                   Pop-Tunes
			     Ad Lib Inc, July 1988

=============================================================================


IMPORTANT
If you are executing from a floppy disk drive, use the /A parameter when
loading Pop-Tunes.  If this parameter is not present, Pop-Tunes may
occasionally have problems opening some of the files it uses.
   A> POPTUNES /A    or...
   C> a:POPTUNES /A

Always load Pop-Tunes before other Ad Lib software so that Pop-Tunes can
set-up the larger buffer that it uses.  Otherwise, the sound driver buffer
may be too small to accept the tunes you wish to play.


=============================================================================
Incompatibilities (version 1.0)
--------------------------------

The following is a list of programs which are known to be partially or
entirely incompatible with Pop-Tunes.

- chkdsk
  100% incompatible.  If you execute it while Pop-Tunes is installed, your
  computer will hang.

- Microsoft Quick C
  Execute Pop-Tunes with the option /X if you plan to use the Quick C editor.

- Using a mouse
  Do not move your mouse while Pop-Tunes is active as this may modify your
  screen when you return to the application.

- Animation games
  Animation games often use the clock, which could cause either the game or
  Pop-Tunes to slow down.  Also note that several games developers will be
  supporting our card in the near future, in which case Pop-Tunes should not
  be used because of conflicts accessing the card.

- MGA card (Hercules)
  Will not work in graphics mode.  No problem with text screens.

- EGA graphics screens
  If Pop-Tunes is activated while you are in one of the EGA graphics
  modes, a reduced screen will appear in the top, left hand corner.
  This screen will be slightly different and the help screens will not
  be available.
  If the application program has programmed the EGA card directly, Pop-Tunes
  may be displayed with unusual color combinations (i.e. white on white) and
  the application screen's colors may be changed when you exit Pop-Tunes.
  Your screen will return to normal the next time the application program
  refreshes the screen.
  Avoid activating Pop-Tunes while a program is writing an image on the
  screen.  Wait until the screen is quiet.
  If you wish to use the regular Pop-Tunes screen, you may be able to switch
  to a CGA mode depending on your graphics card and the application you are
  running.
