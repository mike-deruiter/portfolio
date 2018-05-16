       IDENTIFICATION DIVISION.
       PROGRAM-ID. MUSIC-COLLECTION.
       AUTHOR. MIKE DERUITER.
       
       ENVIRONMENT DIVISION.
       CONFIGURATION SECTION.
       SOURCE-COMPUTER. GNUCOBOL.
       
       INPUT-OUTPUT SECTION.
       FILE-CONTROL.
            SELECT IP-MUSIC
                ASSIGN TO "/tmp/MUSIC.DAT"
                ORGANIZATION IS LINE SEQUENTIAL.
            SELECT OP-COLLECTION
                ASSIGN TO DISPLAY
                ORGANIZATION IS LINE SEQUENTIAL.
            SELECT SW-MUSIC
                ORGANIZATION IS LINE SEQUENTIAL.

       DATA DIVISION.
        
       FILE SECTION.
       FD  IP-MUSIC.
       01  IP-MUSIC-REC                 PIC X(52).

       SD  SW-MUSIC.
       01  SW-MUSIC-REC.
            05  SW-MUSIC-ARTIST         PIC X(20).
            05  SW-MUSIC-TITLE          PIC X(20).
            05  SW-MUSIC-YEAR           PIC X(04).
            05  SW-MUSIC-VALUE          PIC 999V99.
            05  SW-MUSIC-RATING         PIC 9V99.

       FD  OP-COLLECTION.
       01  OP-COLLECTION-REC.              
            05                          PIC X(06).
            05  OP-MUSIC-YEAR            PIC X(04).
            05                          PIC X(03).
            05  OP-MUSIC-ARTIST          PIC X(20).
            05                          PIC X(02).
            05  OP-MUSIC-TITLE           PIC X(20).
            05                          PIC X(03).
            05  OP-MUSIC-VALUE           PIC $$$9.99.
            05                          PIC X(05).
            05  OP-MUSIC-RATING          PIC 9.99.

       WORKING-STORAGE SECTION.
       01  WS-ACCUMULATORS.
            05  A-TOTAL-MUSIC-VALUE      PIC 9999V99 VALUE ZERO.
            05  A-TOTAL-MUSIC-RATING     PIC 999V99 VALUE ZERO.
            05  A-TOTAL-MUSIC-NUM        PIC 9999 VALUE ZERO.
            05  A-YEAR-MUSIC-VALUE       PIC 9999V99 VALUE ZERO.
            05  A-YEAR-MUSIC-RATING      PIC 999V99 VALUE ZERO.
            05  A-YEAR-MUSIC-NUM         PIC 9999 VALUE ZERO.

       01  WS-SWITCHES.
            05  EOF-IP-MUSIC-SW         PIC X VALUE "N".
                88  EOF-IP-MUSIC              VALUE "Y".            
        
       01  WS-AVERAGES.
            05  WS-AVG-MUSIC-RATING      PIC 999V99 VALUE ZERO.
            05  WS-AVG-MUSIC-VAL         PIC 9999V99 VALUE ZERO.
        
       01  WS-HIGHEST-LOWEST.
            05  WS-HIGHEST-RATING   PIC 999V99 VALUE ZERO.
            05  WS-LOWEST-RATING    PIC 999V99 VALUE 5.
        
       01  WS-TEMPORARY.
            05  WS-PREV-YEAR            PIC X(04) VALUE ALL SPACES.
        
       01  YEAR-AVERAGE-REC.
            05                          PIC X(06) VALUE ALL SPACES.
            05  YA-YEAR                 PIC X(05) VALUE ALL SPACES.
            05                          PIC X(08) VALUE "AVERAGE:".
            05                          PIC X(40) VALUE ALL SPACES.
            05  YA-MUSIC-VALUE          PIC $$9.99 VALUE ZERO.
            05                          PIC X(05) VALUE ALL SPACES.
            05  YA-MUSIC-RATING         PIC 9.99 VALUE ZERO.
            
       01  YEAR-TOTAL-REC.
            05                          PIC X(06) VALUE ALL SPACES.
            05  YT-YEAR                 PIC X(05) VALUE ALL SPACES.
            05                          PIC X(06) VALUE "TOTAL:".
            05                          PIC X(40) VALUE ALL SPACES.
            05  YT-MUSIC-VALUE          PIC $,$$9.99 VALUE ZERO.
        
       01  GRAND-AVERAGE-REC.
            05                          PIC X(06) VALUE ALL SPACES.
            05                          PIC X(06) VALUE "GRAND ".
            05                          PIC X(08) VALUE "AVERAGE:".
            05                          PIC X(39) VALUE ALL SPACES.
            05  GA-MUSIC-VALUE          PIC $$9.99 VALUE ZERO.
            05                          PIC X(05) VALUE ALL SPACES.
            05  GA-MUSIC-RATING         PIC 9.99 VALUE ZERO.

       01  GRAND-TOTAL-REC.
            05                          PIC X(06) VALUE ALL SPACES.
            05                          PIC X(06) VALUE "GRAND ".
            05                          PIC X(06) VALUE "TOTAL:".
            05                          PIC X(39) VALUE ALL SPACES.
            05  GT-MUSIC-VALUE          PIC $,$$9.99 VALUE ZERO.
            
       01  HIGHEST-YEAR-REC.
            05                          PIC X(06) VALUE ALL SPACES.
            05                          PIC X(11) VALUE "BEST YEAR: ".
            05                          PIC X(53) VALUE SPACES.
            05  HY-YEAR                 PIC X(04) VALUE SPACES.
            
       01  LOWEST-YEAR-REC.
            05                          PIC X(06) VALUE ALL SPACES.
            05                          PIC X(11) VALUE "WORST YEAR:".
            05                          PIC X(53) VALUE SPACES.
            05  LY-YEAR                 PIC X(04) VALUE SPACES.
            
       01  TITLE-1.
            05                          PIC X(28) VALUE ALL SPACES.
            05                          PIC X(13) VALUE "MIKE'S MUSIC ".
            05                          PIC X(10) VALUE "COLLECTION".

       01  HEADER-1.
            05                          PIC X(06) VALUE ALL SPACES.
            05                          PIC X(04) VALUE "YEAR".
            05                          PIC X(03) VALUE ALL SPACES.
            05                          PIC X(20) VALUE "ARTIST".
            05                          PIC X(02) VALUE ALL SPACES.
            05                          PIC X(20) VALUE "TITLE".
            05                          PIC X(05) VALUE ALL SPACES.
            05                          PIC X(05) VALUE "VALUE".
            05                          PIC X(03) VALUE ALL SPACES.
            05                          PIC X(06) VALUE "RATING".

       01  HEADER-DASHES.
            05                          PIC X(06) VALUE ALL SPACES.
            05                          PIC X(04) VALUE ALL "-".
            05                          PIC X(03) VALUE ALL SPACES.
            05                          PIC X(20) VALUE ALL "-".
            05                          PIC X(02) VALUE ALL SPACES.
            05                          PIC X(20) VALUE ALL "-".
            05                          PIC X(03) VALUE ALL SPACES.
            05                          PIC X(07) VALUE ALL "-".
            05                          PIC X(03) VALUE ALL SPACES.
            05                          PIC X(06) VALUE ALL "-".

       01  FOOTER-DASHES.
            05                          PIC X(58) VALUE ALL SPACES.
            05                          PIC X(07) VALUE ALL "-".
            05                          PIC X(03) VALUE ALL SPACES.
            05                          PIC X(06) VALUE ALL "-".
            
       01  GRAND-TOTAL-AVG-DASHES.
            05                          PIC X(06) VALUE ALL SPACES.
            05                          PIC X(68) VALUE ALL "-".

       PROCEDURE DIVISION.

       00000-MAIN.
            SORT SW-MUSIC
                ASCENDING SW-MUSIC-YEAR,
                          SW-MUSIC-ARTIST,
                          SW-MUSIC-TITLE
                USING IP-MUSIC
                OUTPUT PROCEDURE OA1000-MAINLINE.

            STOP RUN.
            
       OA1000-MAINLINE.
            PERFORM OB1000-SETUP.
            PERFORM OB2000-PROCESS
                UNTIL EOF-IP-MUSIC.
            PERFORM OB3000-WRAPUP.

       OB1000-SETUP.
            OPEN OUTPUT OP-COLLECTION.

            WRITE OP-COLLECTION-REC FROM TITLE-1
                BEFORE ADVANCING 2 LINES.
            WRITE OP-COLLECTION-REC FROM GRAND-TOTAL-AVG-DASHES
                BEFORE ADVANCING 2 LINES.
            WRITE OP-COLLECTION-REC FROM HEADER-1
                BEFORE ADVANCING 1 LINE.
            WRITE OP-COLLECTION-REC FROM HEADER-DASHES
                BEFORE ADVANCING 1 LINE.

            MOVE SPACES TO OP-COLLECTION-REC.

            RETURN SW-MUSIC
                AT END MOVE "Y" TO EOF-IP-MUSIC-SW.
            
            MOVE SW-MUSIC-YEAR          TO WS-PREV-YEAR.
            MOVE SW-MUSIC-YEAR          TO OP-MUSIC-YEAR.
            
            MOVE SW-MUSIC-YEAR          TO HY-YEAR.
            MOVE SW-MUSIC-YEAR          TO LY-YEAR.

       OB2000-PROCESS.
            IF SW-MUSIC-YEAR IS NOT EQUAL TO WS-PREV-YEAR
                PERFORM OC2000-PRINT-PAGE-TOTAL-AVG
                MOVE SW-MUSIC-YEAR      TO WS-PREV-YEAR
                MOVE SW-MUSIC-YEAR      TO OP-MUSIC-YEAR
            END-IF.
       
            MOVE SW-MUSIC-TITLE         TO OP-MUSIC-TITLE.
            MOVE SW-MUSIC-ARTIST        TO OP-MUSIC-ARTIST.
            MOVE SW-MUSIC-VALUE         TO OP-MUSIC-VALUE.
            MOVE SW-MUSIC-RATING        TO OP-MUSIC-RATING.

            ADD  SW-MUSIC-VALUE         TO A-YEAR-MUSIC-VALUE.
            ADD  SW-MUSIC-RATING        TO A-YEAR-MUSIC-RATING.
            ADD  1                      TO A-YEAR-MUSIC-NUM.

            WRITE OP-COLLECTION-REC
                BEFORE ADVANCING 1 LINE.

            MOVE SPACES                 TO OP-MUSIC-YEAR.

            RETURN SW-MUSIC
                AT END MOVE "Y" TO EOF-IP-MUSIC-SW.

       OB3000-WRAPUP.
            PERFORM OC2000-PRINT-PAGE-TOTAL-AVG
            PERFORM OC3000-PRINT-GRAND-TOTAL-AVG.
            
            CLOSE OP-COLLECTION.
                
       OC2000-PRINT-PAGE-TOTAL-AVG.
            PERFORM OD1000-CALC-YEAR-TOTAL-AVG.
       
            MOVE WS-PREV-YEAR           TO YA-YEAR.
            MOVE WS-PREV-YEAR           TO YT-YEAR.
            
            WRITE OP-COLLECTION-REC FROM FOOTER-DASHES
                BEFORE ADVANCING 1 LINE.
            WRITE OP-COLLECTION-REC FROM YEAR-AVERAGE-REC
                BEFORE ADVANCING 1 LINE.
            WRITE OP-COLLECTION-REC FROM YEAR-TOTAL-REC
                BEFORE ADVANCING 2 LINES.
                
            MOVE SPACES                 TO OP-COLLECTION-REC.
                
       OC3000-PRINT-GRAND-TOTAL-AVG.
            PERFORM OD2000-CALC-GRAND-TOTAL-AVG.
            
            WRITE OP-COLLECTION-REC FROM GRAND-TOTAL-AVG-DASHES
                BEFORE ADVANCING 1 LINE.
            WRITE OP-COLLECTION-REC FROM GRAND-TOTAL-AVG-DASHES
                BEFORE ADVANCING 2 LINES.
            WRITE OP-COLLECTION-REC FROM HIGHEST-YEAR-REC
                BEFORE ADVANCING 1 LINE.
            WRITE OP-COLLECTION-REC FROM LOWEST-YEAR-REC
                BEFORE ADVANCING 2 LINES.
            WRITE OP-COLLECTION-REC FROM GRAND-AVERAGE-REC
                BEFORE ADVANCING 1 LINE.
            WRITE OP-COLLECTION-REC FROM GRAND-TOTAL-REC
                BEFORE ADVANCING 1 LINE.

            MOVE SPACES                 TO OP-COLLECTION-REC.
                
       OD1000-CALC-YEAR-TOTAL-AVG.
            MOVE A-YEAR-MUSIC-VALUE      TO YT-MUSIC-VALUE.
            
            MOVE A-YEAR-MUSIC-VALUE      TO WS-AVG-MUSIC-VAL.
            MOVE A-YEAR-MUSIC-RATING     TO WS-AVG-MUSIC-RATING.
            
            DIVIDE A-YEAR-MUSIC-NUM INTO WS-AVG-MUSIC-VAL
                GIVING YA-MUSIC-VALUE.
            DIVIDE A-YEAR-MUSIC-NUM INTO WS-AVG-MUSIC-RATING
                GIVING YA-MUSIC-RATING.
                
            IF YA-MUSIC-RATING > WS-HIGHEST-RATING
                MOVE YA-MUSIC-RATING    TO WS-HIGHEST-RATING
                MOVE SW-MUSIC-YEAR      TO HY-YEAR
            ELSE IF YA-MUSIC-RATING < WS-LOWEST-RATING
                MOVE YA-MUSIC-RATING    TO WS-LOWEST-RATING
                MOVE SW-MUSIC-YEAR      TO LY-YEAR
            END-IF.
                
            ADD  A-YEAR-MUSIC-VALUE      TO A-TOTAL-MUSIC-VALUE.
            ADD  A-YEAR-MUSIC-RATING     TO A-TOTAL-MUSIC-RATING.
            ADD  A-YEAR-MUSIC-NUM        TO A-TOTAL-MUSIC-NUM.    
                
            MOVE ZEROES                 TO A-YEAR-MUSIC-VALUE.
            MOVE ZEROES                 TO A-YEAR-MUSIC-RATING.
            MOVE ZEROES                 TO A-YEAR-MUSIC-NUM.

                
       OD2000-CALC-GRAND-TOTAL-AVG.
            MOVE A-TOTAL-MUSIC-VALUE TO GT-MUSIC-VALUE.
            
            MOVE A-TOTAL-MUSIC-VALUE     TO WS-AVG-MUSIC-VAL.
            MOVE A-TOTAL-MUSIC-RATING    TO WS-AVG-MUSIC-RATING.
            
            DIVIDE A-TOTAL-MUSIC-NUM INTO WS-AVG-MUSIC-VAL
                GIVING GA-MUSIC-VALUE.
            DIVIDE A-TOTAL-MUSIC-NUM INTO WS-AVG-MUSIC-RATING
                GIVING GA-MUSIC-RATING.
