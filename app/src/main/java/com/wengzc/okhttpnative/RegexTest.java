package com.wengzc.okhttpnative;

import android.util.Log;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class RegexTest {

    private static final String TOKEN = "([a-zA-Z0-9-!#$%&'*+.^_`{|}~]+)";
    private static final String QUOTED= "\"([^\"]*)\"";
    private static final Pattern TYPE_SUBTYPE = Pattern.compile(TOKEN + "/" + TOKEN);
    private static final Pattern PARAMETER = Pattern.compile(
        ";\\s*(?>"+ TOKEN + "=(?>" + TOKEN+ "|" + QUOTED +"))?"
    );

    private static void assertMatch (){
        try{
            String testString = "text/plain; charset=us-ascii";
            //String testString = "text/plain";
            Matcher typeSubtype = TYPE_SUBTYPE.matcher(testString);
            typeSubtype.lookingAt();
            String type = typeSubtype.group(1);
            String subtype = typeSubtype.group(2);

            String charset = null;
            Matcher parameter = PARAMETER.matcher(testString);
            for (int s = typeSubtype.end(); s < testString.length(); s = parameter.end()){
                parameter.region(s, testString.length());
                parameter.lookingAt();
                String name = parameter.group(1);
                String charsetParameter;
                String token = parameter.group(2);
                if (token != null){
                    charsetParameter = (token.startsWith("'") && token.endsWith("'") && token.length() > 2)
                            ? token.substring(1, token.length() - 1)
                            : token;
                }else{
                    charsetParameter = parameter.group(3);
                }

                charset = charsetParameter;
            }
        }catch (Exception e){
            e.printStackTrace();
        }
    }

    public static void main (String[] args){

        Matcher matcher1 = Pattern.compile("(\\w*)(?<=b)re(?=(g)|(s))(\\w*)").matcher("a regular expression reg breg brest ");
        while (matcher1.find()){
            String substr = matcher1.group();
            String g1 = matcher1.group(1);
            String g2 = matcher1.group(2);
            String g3 = matcher1.group(3);
            String g4 = matcher1.group(4);
            System.out.println(substr);
        }

    }


}
