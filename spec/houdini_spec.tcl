package require "Houdini"

describe "::Houdini" {
    describe "::escape_html" {
        describe "with -unsecure flag" {
            it "does not escape forward slashes" {
                expect [
                    Houdini::escape_html -unsecure "<some_tag/>"
                ] to equal "&lt;some_tag/&gt;"
            }
        }

        it "escapes basic html" {
            expect [
                Houdini::escape_html "<some_tag/>"
            ] to equal "&lt;some_tag&#47;&gt;"
        }

        it "escapes double quotes" {
            expect [
                Houdini::escape_html "<some_tag some_attr=\"some value\"/>"
            ] to equal "&lt;some_tag some_attr=&quot;some value&quot;&#47;&gt;"
        }

        it "escapes single quotes" {
            expect [
                Houdini::escape_html "<some_tag some_attr='some value'/>"
            ] to equal "&lt;some_tag some_attr=&#39;some value&#39;&#47;&gt;"
        }

        it "escapes ampersands" {
            expect [
                Houdini::escape_html "<b>Bourbon & Branch</b>"
            ] to equal "&lt;b&gt;Bourbon &amp; Branch&lt;&#47;b&gt;"
        }

        it "returns original content if no escaping is necessary" {
            expect [Houdini::escape_html "FooBar"] to equal "FooBar"
        }
    }

    describe "::escape_js" {
        it "escapes quotes and newlines" {
            expect [
                Houdini::escape_js "This \"thing\" is really\n neato\r\n\n'"
            ] to equal "This \\\"thing\\\" is really\\n neato\\n\\n\\'"
        }

        it "escapes backslashes" {
            expect [
                Houdini::escape_js "backslash\\test"
            ] to equal "backslash\\\\test"
        }

        it "escapes closing html tags" {
            expect [
                Houdini::escape_js "keep <open>, but dont </close> tags"
            ] to equal "keep <open>, but dont <\\/close> tags"
        }
    }

    describe "::escape_url" {
        it "escapes basic urls" {
            expect [
                Houdini::escape_url "http://www.homerun.com/"
            ] to equal "http%3A%2F%2Fwww.homerun.com%2F"
        }

        it "escapes urls containing tags" {
            expect [
                Houdini::escape_url "fo<o>bar"
            ] to equal "fo%3Co%3Ebar"
        }

        it "escapes urls containing whitespace" {
            expect [
                Houdini::escape_url "a space"
            ] to equal "a+space"

            expect [
                Houdini::escape_url "a   sp ace "
            ] to equal "a+++sp+ace+"
        }

        it "escapes urls with mixed characters" {
            expect [
                Houdini::escape_url "q1!2\"'w\$5&7/z8)?\\"
            ] to equal "q1%212%22%27w%245%267%2Fz8%29%3F%5C"
        }

        it "escapes urls with multibyte characters" {
            # Matsumoto
            expect [
                Houdini::escape_url [encoding convertfrom "utf-8" "\xE3\x81\xBE\xE3\x81\xA4\xE3\x82\x82\xE3\x81\xA8"]
            ] to equal "%E3%81%BE%E3%81%A4%E3%82%82%E3%81%A8"

            # Matsu moto
            expect [
                Houdini::escape_url [encoding convertfrom "utf-8" "\xE3\x81\xBE\xE3\x81\xA4 \xE3\x82\x82\xE3\x81\xA8"]
            ] to equal "%E3%81%BE%E3%81%A4+%E3%82%82%E3%81%A8"
        }
    }

    describe "::escape_uri" {
        it "escapes uris containing tags" {
            expect [
                Houdini::escape_uri "fo<o>bar"
            ] to equal "fo%3Co%3Ebar"
        }

        it "escapes uris containing whitespace" {
            expect [
                Houdini::escape_uri "a space"
            ] to equal "a%20space"

            expect [
                Houdini::escape_uri "a   sp ace "
            ] to equal "a%20%20%20sp%20ace%20"
        }

        it "escapes uris with mixed characters" {
            expect [
                Houdini::escape_uri "q1!2\"'w\$5&7/z8)?\\"
            ] to equal "q1!2%22'w\$5&7/z8)?%5C"
        }

        it "escapes uris with multibyte characters" {
            # Matsumoto
            expect [
                Houdini::escape_uri [encoding convertfrom "utf-8" "\xE3\x81\xBE\xE3\x81\xA4\xE3\x82\x82\xE3\x81\xA8"]
            ] to equal "%E3%81%BE%E3%81%A4%E3%82%82%E3%81%A8"

            # Matsu moto
            expect [
                Houdini::escape_uri [encoding convertfrom "utf-8" "\xE3\x81\xBE\xE3\x81\xA4 \xE3\x82\x82\xE3\x81\xA8"]
            ] to equal "%E3%81%BE%E3%81%A4%20%E3%82%82%E3%81%A8"
        }
    }
}
