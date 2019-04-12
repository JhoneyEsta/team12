import events.FullMessageEvent;
import events.onjoinEvent;
import net.dv8tion.jda.api.JDA;
import net.dv8tion.jda.api.JDABuilder;
import net.dv8tion.jda.api.events.guild.GuildJoinEvent;

public class Main {

    public static void main(String args[]) throws Exception{

        JDA jda = new JDABuilder( "NTQ5OTAxNjQxNDQ5NjAzMDkz.XKy1uw.Lni-h-pKyDzlGllbEPs0DsouMkQ").build();

        jda.addEventListener(new onjoinEvent());
        //jda.addEventListener(new HelloEvent());
        jda.addEventListener(new FullMessageEvent());

    }
}