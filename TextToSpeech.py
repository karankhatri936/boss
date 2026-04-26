import speech_recognition as sr
import subprocess


recognizer = sr.Recognizer()

while True:
    with sr.Microphone() as source:
        print("\nListening... (say 'exit' to stop):")

        recognizer.adjust_for_ambient_noise(source, duration=0.5)
        audio = recognizer.listen(source, timeout=5, phrase_time_limit=10)

        try:
            input = recognizer.recognize_google(audio).lower()
            # print("You said: ", input) 

            if "exit" in input:
                print("Stopping boss...")
                break

            elif "boss" in input:
                print("Boss is listening...")

                    # recognizer.adjust_for_ambient_noise(source, duration=0.5)

                try:
                    audio = recognizer.listen(source, timeout=5, phrase_time_limit=10)
                    command = recognizer.recognize_google(audio).lower()
                    print("Processing your command...")


                except:
                    print("Could not understand, try again.")
                

            with open("command.txt", "w") as file:
                file.write(command)

            subprocess.run(["boss.exe"])
            command = ""

        except:
            print("Could not understand, try again.")
